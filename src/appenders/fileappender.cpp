/**
 * @file fileappender.cpp
 * @brief File appender class.
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

#include "fileappender.h"

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
fileAppender::fileAppender() :
    _Mybase()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
fileAppender::~fileAppender()
{
    close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::string fileAppender::file() const
{
    return _Mybase::prop<std::string>( PROP_FILE );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void fileAppender::setFile( const std::string& value )
{
    _Mybase::setProp( PROP_FILE, value );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool fileAppender::appendToFile() const
{
    return _Mybase::prop<bool>( PROP_APPENDTOFILE );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void fileAppender::setAppendToFile( bool value )
{
    _Mybase::setProp( PROP_APPENDTOFILE, value );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::size_t fileAppender::pos() const
{
    return ( (std::size_t) const_cast<std::ofstream*>( &ofs_ )->tellp() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool fileAppender::open()
{
    // retrieve filename
    std::string filename( file() );

    if ( filename.empty() )
        return false;

    // open!
    ofs_.open( filename.c_str(), std::ofstream::out | (appendToFile() ? std::ofstream::app : std::ofstream::trunc) );

    return ofs_.is_open();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void fileAppender::close()
{
    if ( ofs_.is_open() )
        ofs_.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void fileAppender::write( const std::string& line )
{
    ofs_ << line;
    ofs_.flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio


