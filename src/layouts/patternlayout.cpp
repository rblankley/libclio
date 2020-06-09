/**
 * @file patternlayout.cpp
 * @brief Pattern layout class.
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

#include "patternlayout.h"

#include "../logline.h"

#include <cstdio>
#include <ctime>
#include <locale>
#include <stdexcept>
#include <sstream>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
patternLayout::patternLayout() :
    _Mybase()
{
    // create list of all formats
    formats_.push_back( "%date" );
    formats_.push_back( "%epochms" );
    formats_.push_back( "%epoch" );
    formats_.push_back( "%thread" );
    formats_.push_back( "%levelnum" );
    formats_.push_back( "%level" );
    formats_.push_back( "%module" );
    formats_.push_back( "%class" );
    formats_.push_back( "%method" );
    formats_.push_back( "%message" );
    formats_.push_back( "%file" );
    formats_.push_back( "%linenum" );
    formats_.push_back( "%newline" );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
patternLayout::~patternLayout()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::string patternLayout::conversionPattern() const
{
    return _Mybase::prop<std::string>( PROP_CONVERSIONPATTERN );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void patternLayout::setConversionPattern( const std::string& value )
{
    _Mybase::setProp( PROP_CONVERSIONPATTERN, value );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::string patternLayout::format( const logLine& line ) const
{
    std::string result( conversionPattern() );

    // no pattern
    if ( result.empty() )
        return _Mybase::format( line );

    std::string name;
    std::string format;

    // formats
    while ( findFormat( result, name, format ) )
    {
        char value[512];

        // validate format
        if ( !format.length() )
        {
            // date format
            if ( std::string::npos != name.find( "%date" ) )
                format = DEFAULT_DATE_FORMAT;

            // long numeric format
            else if ( std::string::npos != name.find( "%epochms" ) )
                format = DEFAULT_LARGE_NUMBER_FORMAT;

            // numeric format
            else if (( std::string::npos != name.find( "%thread" ) ) ||
                ( std::string::npos != name.find( "%levelnum" ) ) ||
                ( std::string::npos != name.find( "%linenum" ) ) ||
                ( std::string::npos != name.find( "%epoch" ) ))
                format = DEFAULT_NUMBER_FORMAT;

            // string format
            else
                format = DEFAULT_STRING_FORMAT;
        }

        // create formatted string
        if ( std::string::npos != name.find( "%date" ) )
        {
            std::string::size_type pos;

            std::time_t temp( logLine::clock_type::to_time_t( line.timeStamp() ) );

            // look for milliseconds
            if ( std::string::npos != (pos = format.find( "%L" )) )
            {
                logLine::clock_type::time_point rounded( logLine::clock_type::from_time_t( temp ) );

                if ( rounded > line.timeStamp() )
                {
                    --temp;
                    rounded -= std::chrono::seconds( 1 );
                }

                char milliseconds[32];
                std::snprintf( milliseconds, sizeof(milliseconds), "%03d", (int) std::chrono::duration_cast<std::chrono::milliseconds>( line.timeStamp() - rounded ).count() );

                // replace
                format.replace( pos, 2, milliseconds );
            }

            // format time
            tm stamp;
#if _WIN32
            ::localtime_s( &stamp, &temp );
#else
            ::localtime_r( &temp, &stamp );
#endif

            std::strftime( value, sizeof( value ), format.c_str(), &stamp );
        }

        else if ( std::string::npos != name.find( "%epochms" ) )
        {
            const std::chrono::milliseconds epoch( std::chrono::duration_cast<std::chrono::milliseconds>( line.timeStamp().time_since_epoch() ) );
            std::snprintf( value, sizeof(value), format.c_str(), epoch.count() );
        }

        else if ( std::string::npos != name.find( "%epoch" ) )
        {
            const std::chrono::seconds epoch( std::chrono::duration_cast<std::chrono::seconds>( line.timeStamp().time_since_epoch() ) );
            std::snprintf( value, sizeof(value), format.c_str(), epoch.count() );
        }

        else if ( std::string::npos != name.find( "%thread" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.threadId() );

        else if ( std::string::npos != name.find( "%levelnum" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.level() );

        else if ( std::string::npos != name.find( "%level" ) )
        {
            std::string temp( logLevel::toString( line.level() ) );
            std::snprintf( value, sizeof(value), format.c_str(), temp.c_str() );
        }

        else if ( std::string::npos != name.find( "%module" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.moduleName().c_str() );

        else if ( std::string::npos != name.find( "%class" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.className().c_str() );

        else if ( std::string::npos != name.find( "%method" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.classFunction().c_str() );

        else if ( std::string::npos != name.find( "%message" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.text().c_str() );

        else if ( std::string::npos != name.find( "%file" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.sourceFilename().c_str() );

        else if ( std::string::npos != name.find( "%linenum" ) )
            std::snprintf( value, sizeof(value), format.c_str(), line.sourceLine() );

        else if ( std::string::npos != name.find( "%newline" ) )
        {
            std::stringstream temp;
            temp << std::endl;

            std::snprintf( value, sizeof(value), format.c_str(), temp.str().c_str() );
        }

        // replace format with formatted string
        std::string::size_type pos;

        while ( std::string::npos != (pos = result.find( name )) )
            result.replace( pos, name.size(), value );
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool patternLayout::findFormat( const std::string& line, std::string& name, std::string& format ) const
{
    std::string::size_type pos( std::string::npos );

    format.clear();

    // loop through each format, find match
    for ( stringList::const_iterator i = formats_.begin(); i != formats_.end(); ++i )
        if ( std::string::npos != (pos = line.find( *i )) )
        {
            name = *i;
            break;
        }

    if ( std::string::npos == pos )
        return false;

    try
    {
        std::string::size_type nameLength( name.length() );

        // look for formatting override
        if ( "{" == line.substr( pos + nameLength, 1 ) )
        {
            std::string::size_type end;

            // found!
            if ( std::string::npos != (end = line.find( "}", pos )) )
            {
                name = line.substr( pos, end - pos + 1 );
                format = name.substr( nameLength + 1, name.length() - nameLength - 2 );
            }
        }
    }
    catch ( const std::out_of_range& /*ex*/ )
    {
        // this exception is okay, no formatting present...
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio



