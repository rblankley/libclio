/**
 * @file rollingfileappender.cpp
 * @brief File appender that rolls when reaching specified size.
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

#include "rollingfileappender.h"

#include <cstdio>
#include <iostream>
#include <string>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
rollingFileAppender::rollingFileAppender() :
    _Mybase()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
rollingFileAppender::~rollingFileAppender()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::size_t rollingFileAppender::maxSizeRollBackups() const
{
    return _Mybase::prop<std::size_t>( PROP_MAXSIZEROLLBACKUPS );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void rollingFileAppender::setMaxSizeRollBackups( std::size_t value )
{
    _Mybase::setProp( PROP_MAXSIZEROLLBACKUPS, value );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::size_t rollingFileAppender::maximumFileSize() const
{
    return _Mybase::prop<std::size_t>( PROP_MAXIMUMFILESIZE );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void rollingFileAppender::setMaximumFileSize( std::size_t value )
{
    _Mybase::setProp( PROP_MAXIMUMFILESIZE, value );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void rollingFileAppender::write( const std::string& line )
{
    // check if should roll
    if ( shouldRollLogs() )
        rollLogs();

    _Mybase::write( line );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool rollingFileAppender::shouldRollLogs() const
{
    std::size_t maxSize( 1048576 * maximumFileSize() ); // convert to MB

    return (( maxSize ) && ( maxSize <= _Mybase::pos() ));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void rollingFileAppender::rollLogs()
{
    // close log
    close();

    // retrieve filename and number of copies to keep
    std::string filename( file() );
    std::size_t copies( maxSizeRollBackups() );

    // roll logs
    if ( !copies )
        std::remove( filename.c_str() );
    else
    {
        do
        {
            std::string old_filename( filename );
            std::string new_filename( filename );

            if ( 1 < copies )
            {
                old_filename.append( "." );
                old_filename.append( std::to_string( copies-1 ) );
            }

            new_filename.append( "." );
            new_filename.append( std::to_string( copies ) );

            std::remove( new_filename.c_str() );
            std::rename( old_filename.c_str(), new_filename.c_str() );

        } while ( --copies );
    }

    // open log
    open();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio
