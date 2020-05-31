/**
 * @file loggermanager.cpp
 * @brief Logger manager class.
 *
 * @section Copyright
 * Copyright (C) 2013 Randy Blankley
 *
 * @section License
 * This file is part of libclio.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "appender.h"
#include "appenderfactory.h"
#include "layout.h"
#include "layoutfactory.h"
#include "logger.h"
#include "loggermanager.h"
#include "tinyxml2.h"

#include <sys/stat.h>

/// Clio namespace.
namespace clio
{

loggerManagerPtr loggerManager::instance_( nullptr );
std::mutex loggerManager::instanceMutex_;

///////////////////////////////////////////////////////////////////////////////////////////////////
loggerManager::loggerManager() :
    refreshInterval_( DEFAULT_REFRESH_INTERVAL ),
    monitorThread_( [this] {monitorConfiguration();} ),
    rootLogger_( new logger() )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
loggerManager::~loggerManager()
{
    {
#if HAVE_CXX17 || HAVE_CXX14
        std::unique_lock<mutex> lock( m_ );
#else
        std::lock_guard<mutex> lock( m_ );
#endif

        // exit monitor thread
        stopMonitoring_.notify_all();

        // cleanup
        cleanup();
    }

    // wait for monitor thread to terminate
    monitorThread_.join();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
loggerWeakPtr loggerManager::find( const std::string& name ) const
{
#if HAVE_CXX17 || HAVE_CXX14
    std::shared_lock<mutex> lock( m_ );
#else
    std::lock_guard<mutex> lock( m_ );
#endif

    // default to root logger
    loggerPtr log( rootLogger_ );

    // look for specific logger
    for ( const auto& i: loggers_ )
    {
        std::string loggerName( i.first );

        // compare names... logger name may have wildcards!
        if ( wildCompare( loggerName.c_str(), name.c_str() ) )
        {
            // found one!
            log = i.second;
            break;
        }
    }

    return log;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool loggerManager::configure( const std::string& filename )
{
#if HAVE_CXX17 || HAVE_CXX14
    std::unique_lock<mutex> lock( m_ );
#else
    std::lock_guard<mutex> lock( m_ );
#endif

    // nothing to do
    if ( filename == configFile_ )
        return true;

    // set configuration
    if ( setConfiguration( filename ) )
    {
        configFile_ = filename;
        configFileModifiedTime_ = fileModifiedTime( configFile_ );
        configFileSize_ = fileSize( configFile_ );
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
loggerManagerPtr loggerManager::instance()
{
    if ( !instance_ )
    {
        std::lock_guard<std::mutex> guard( instanceMutex_ );

        if ( !instance_ )
            instance_.reset( new loggerManager() );
    }

    return instance_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loggerManager::terminate()
{
    if ( instance_ )
    {
        std::lock_guard<std::mutex> guard( instanceMutex_ );

        if ( instance_ )
            instance_.reset();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Generic property creation.
 */
template <class T>
void loggerManager::createProperty( tinyxml2::XMLElement *prop, T *obj )
{
    if ( !prop->Name() )
        return;

    obj->setProp( std::string( prop->Name() ), std::string( prop->GetText() ? prop->GetText() : "" ) );
}

/**
 * Specialization for appender property creation.
 */
template <>
void loggerManager::createProperty( tinyxml2::XMLElement *prop, appender *obj )
{
    if ( !prop->Name() )
        return;

    std::string propName( prop->Name() );

    // check for layout
    if ( "layout" == propName )
    {
        layout *l( layoutFactory::create( prop->Attribute( "type" ) ) );

        if ( l )
        {
            obj->setFormat( l );

            // process properties
            for ( tinyxml2::XMLElement *lprop = prop->FirstChildElement(); lprop; lprop = lprop->NextSiblingElement() )
                createProperty( lprop, l );
        }
    }

    // normal property
    else
    {
        obj->setProp( propName, std::string( prop->GetText() ? prop->GetText() : "" ) );
    }
}

/**
 * Specialization for logger property creation.
 */
template <>
void loggerManager::createProperty( tinyxml2::XMLElement *prop, logger *obj )
{
    if (( !prop->Name() ) || ( !prop->GetText() ))
        return;

    std::string propName( prop->Name() );
    std::string propValue( prop->GetText() );

    // check for level
    if ( "level" == propName )
        obj->setLevel( logLevel::fromString( propValue ) );

    // check for appender-ref
    else if ( "appender-ref" == propName )
    {
        appenderPtrMap::iterator i( appenders_.find( propValue ) );

        // add appender to logger appender list
        if ( appenders_.end() != i )
        {
            appenderWeakPtrList temp( obj->appenders() );
            temp.push_back( i->second );

            obj->setAppenders( temp );
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loggerManager::createAppender( tinyxml2::XMLElement *app )
{
    const char* name( app->Attribute( "name" ) );
    const char* type( app->Attribute( "type" ) );

    if (( name ) && ( type ))
    {
        appender *a( appenderFactory::create( type ) );

        if ( !a )
            return;

        appenders_.insert( appenderPtrMap::value_type( name, appenderPtr( a ) ) );

        // process properties
        for ( tinyxml2::XMLElement *prop = app->FirstChildElement(); prop; prop = prop->NextSiblingElement() )
            createProperty( prop, a );

        // open appender
        a->open();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loggerManager::createLogger( tinyxml2::XMLElement *log, bool isRoot )
{
    const char* name( log->Attribute( "name" ) );

    if (( isRoot ) || (( !isRoot ) && ( name )))
    {
        logger *l( new logger() );

        if ( isRoot )
            rootLogger_.reset( l );
        else
        {
            loggers_.insert( loggerPtrMap::value_type( name, loggerPtr( l ) ) );
        }

        // process properties
        for ( tinyxml2::XMLElement *prop = log->FirstChildElement(); prop; prop = prop->NextSiblingElement() )
            createProperty( prop, l );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loggerManager::cleanup()
{
    loggers_.clear();
    rootLogger_.reset( new logger() );

    for ( auto& i: appenders_ )
        i.second->close();

    appenders_.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool loggerManager::setConfiguration( const std::string& filename )
{
    tinyxml2::XMLDocument doc;

    // load XML document
    if ( tinyxml2::XML_SUCCESS != doc.LoadFile( filename.c_str() ) )
        return false;

    return setConfiguration( &doc );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool loggerManager::setConfiguration( tinyxml2::XMLDocument *doc )
{
    // find root node
    tinyxml2::XMLElement *root( doc->FirstChildElement( "logging" ) );

    if ( root )
    {
        // remove existing loggers and appenders
        cleanup();

        // setup appenders
        for ( tinyxml2::XMLElement *app = root->FirstChildElement( "appender" ); app; app = app->NextSiblingElement( "appender" ) )
            createAppender( app );

        // setup root logger
        tinyxml2::XMLElement *rootLog = root->FirstChildElement( "root" );

        if ( rootLog )
            createLogger( rootLog, true );

        // setup loggers
        for ( tinyxml2::XMLElement *log = root->FirstChildElement( "logger" ); log; log = log->NextSiblingElement( "logger" ) )
            createLogger( log );

        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
loggerManager::clock_type::duration loggerManager::refreshInterval() const
{
#if HAVE_CXX17 || HAVE_CXX14
    std::shared_lock<mutex> lock( m_ );
#else
    std::lock_guard<mutex> lock( m_ );
#endif

    return refreshInterval_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loggerManager::setRefreshIntervalImpl( const clock_type::duration& interval )
{
    std::lock_guard<mutex> lock( m_ );
    refreshInterval_ = interval;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loggerManager::monitorConfiguration()
{
#if HAVE_CXX17 || HAVE_CXX14
    std::shared_lock<mutex> lock( m_ );
#else
    std::unique_lock<mutex> lock( m_ );
#endif

    clock_type::duration duration;

    do
    {
        if ( configFile_.length() )
        {
            // get modified time and size
            const file_time mt( fileModifiedTime( configFile_ ) );
            const std::uintmax_t s( fileSize( configFile_ ) );

            // check against previous
            if (( configFileModifiedTime_ != mt ) || ( configFileSize_ != s ))
            {
#if HAVE_CXX17 || HAVE_CXX14
                // upgrade our lock
                // why doesn't std c++ support this natively? this code looks awful...
                lock.unlock();

                {
                    std::unique_lock<mutex> lockUpgrade( m_ );
#endif

                    setConfiguration( configFile_ );
                    configFileModifiedTime_ = mt;
                    configFileSize_ = s;

#if HAVE_CXX17 || HAVE_CXX14
                }

                lock.lock();
#endif
            }
        }

        duration = refreshInterval_;

    } while ( std::cv_status::timeout == stopMonitoring_.wait_for( lock, duration ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool loggerManager::wildCompare( const char *wild, const char *s )
{
    // if we reach at the end of both strings, we are done
    if (( *wild == '\0' ) && ( *s == '\0' ))
        return true;
 
    // make sure that the characters after '*' are present in second string. This function assumes
    // that the first string will not contain two consecutive '*'
    if (( *wild == '*' ) && ( *(wild+1) != '\0' ) && ( *s == '\0'))
        return false;
 
    // if the first string contains '?', or current characters of both strings match
    if (( *wild == '?' ) || ( *wild == *s ))
        return wildCompare( wild+1, s+1 );
 
    // if there is *, then there are two possibilities
    // a) we consider current character of second string
    // b) we ignore current character of second string
    if ( *wild == '*' ) 
        return wildCompare( wild+1, s ) || wildCompare( wild, s+1 );

    return false; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
loggerManager::file_time loggerManager::fileModifiedTime( const std::string& filename )
{
#if HAVE_CXX17
    if ( std::filesystem::exists( filename ) )
        return std::filesystem::last_write_time( filename );

    return file_time();
#else
    struct stat stbuf;

    if ( 0 != ::stat( filename.c_str(), &stbuf ) )
        return 0;

    return stbuf.st_mtime;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::uintmax_t loggerManager::fileSize( const std::string& filename )
{
#if HAVE_CXX17
    if ( std::filesystem::exists( filename ) )
        return std::filesystem::file_size( filename );

    return 0;
#else
    struct stat stbuf;

    if ( 0 != ::stat( filename.c_str(), &stbuf ) )
        return 0;

    return stbuf.st_size;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio
