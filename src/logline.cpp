/**
 * @file logline.cpp
 * @brief Log line class.
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

#include "logger.h"
#include "loggermanager.h"
#include "logline.h"

#include <cstdio>
#include <stack>
#include <thread>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
logLine::logLine( logLevel::type level, const char* file, const char* function, unsigned int line ) :
    level_( level ),
    sourceFilename_( file ),
    sourceLine_( line ),
    stamp_( clock_type::now() ),
    threadId_( std::hash<std::thread::id>()( std::this_thread::get_id() ) )
{
    // set logger name from function
    setLoggerName( function );

    if ( level_ < logLevel::Fatal )
        level_ = logLevel::Fatal;
    else if ( logLevel::Trace < level_ )
        level_ = logLevel::Trace;

    // retrieve logger from manager
    logger_ = loggerManager::instance()->find( loggerName() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
logLine::logLine( const _Myt& rhs )
{
    copy( rhs );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
logLine::~logLine()
{
    loggerPtr line( logger_.lock() );

    // write line to log
    if( line )
        line->writeLine( *this );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::setText( const char *format, ... )
{
    std::va_list params;
    va_start( params, format );

    text_.clear();

    appendTextV( format, params );

    va_end( params );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::setTextHex( const void *buffer, unsigned int bufferLen, unsigned int width )
{
    text_.clear();

    appendTextHex( buffer, bufferLen, width );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool logLine::enabled() const
{
    loggerPtr line( logger_.lock() );

    return ( line ? line->enabled( level() ) : false );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::appendText( const char *format, ... )
{
    std::va_list params;
    va_start( params, format );

    appendTextV( format, params );

    va_end( params );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::appendTextHex( const void *buffer, unsigned int bufferLen, unsigned int width )
{
    // log buffer in hex format
    if (( buffer ) && ( bufferLen ))
    {
        const unsigned char *bufferPtr( (const unsigned char*) buffer );

        std::stringstream os;

        std::string readable;
        char temp[512];

        if ( width < MIN_WIDTH )
            width = MIN_WIDTH;
        else if ( width > MAX_WIDTH )
            width = MAX_WIDTH;

        os << std::endl;

        for ( unsigned int i = 0; i < bufferLen; ++i )
        {
            if ( 0 == (i%width) )
            {
                if ( 0 != i )
                {
                    os << readable << std::endl;
                    readable.clear();
                }

                std::sprintf( temp, "    %08X ", i );
                os << temp;
            }

            std::sprintf( temp, "%02X ", bufferPtr[i] );
            os << temp;

            std::sprintf( temp, "%c", (' ' <= bufferPtr[i] && bufferPtr[i] <= '~') ? bufferPtr[i] : '.' );
            readable.append( temp );

            if ( i == (bufferLen-1) )
            {
                while ( 0 != (++i%width) )
                    os << "   ";

                os << readable;
            }
        }

        text_.append( os.str() );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::string logLine::loggerName() const
{
    // generate name from properties
    std::string name;

    if ( !moduleName().empty() )
    {
        name.append( moduleName() );
        name.push_back( '.' );
    }

    if ( !className().empty() )
    {
        name.append( className() );
        name.push_back( '.' );
    }

    name.append( classFunction() );

    return name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::setLoggerName( const char *function )
{
    std::stack<std::string> names;
    std::string name( function );

    // we are looking for the function name only
    // for example the string 'void class::foo( int bar )' we want only 'class::foo'
    std::string::size_type end( name.find( '(' ) );

    if ( std::string::npos != end )
    {
        std::string::size_type begin( name.rfind( ' ', end ) );

        // no space is okay, for example CTOR or DTOR
        if ( std::string::npos == begin )
            begin = 0;

        std::string temp;

        // swap strings as we will extract what we are interested in
        temp.swap( name );

        // push to stack everything between the '::'...
        while ( ++begin < end )
        {
            std::string::value_type c( temp.at( begin ) );

            if ( ':' == c )
            {
                names.push( name );
                name.clear();

                ++begin;
            }
            else
            {
                name.push_back( c );
            }
        }
    }

    // save off name
    names.push( name );

    // we should have list that looks like this now:
    // function name
    // class name
    // namespace c
    // namespace b
    // namespace a
    //
    // lets pop off into our variables...

    classFunction_ = names.top();
    names.pop();

    if ( names.empty() )
        return;

    className_ = names.top();
    names.pop();

    if ( names.empty() )
        return;

    moduleName_ = names.top();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::copy( const _Myt& rhs )
{
    level_ = rhs.level_;

    moduleName_ = rhs.moduleName_;
    className_ = rhs.className_;
    classFunction_ = rhs.classFunction_;

    sourceFilename_ = rhs.sourceFilename_;
    sourceLine_ = rhs.sourceLine_;

    text_ = rhs.text_;

    stamp_ = rhs.stamp_;
    threadId_ = rhs.threadId_;

    logger_ = rhs.logger_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logLine::appendTextV( const char *format, std::va_list params )
{
    char *temp( nullptr );

    // determine how much space to reserve/allocate for string
    int reserveSize( std::vsprintf( temp, format, params ) );

    if ( 0 < reserveSize )
    {
        // allocate string
        temp = new char[reserveSize];

        // format string
        std::vsprintf( temp, format, params );
        text_.append( temp );

        // cleanup
        delete [] temp;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio

