/**
 * @file loglevel.cpp
 * @brief Log level enumeration.
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

#include "loglevel.h"

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
logLevel::type logLevel::fromString( const std::string& value )
{
    if ( toString( Fatal ) == value )
        return Fatal;
    else if ( toString( Error ) == value )
        return Error;
    else if ( toString( Warning ) == value )
        return Warning;
    else if ( toString( Info ) == value )
        return Info;
    else if ( toString( Debug ) == value )
        return Debug;
    else if ( toString( Trace ) == value )
        return Trace;
    else if ( toString( Everything ) == value )
        return Everything;

    return Disabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::string logLevel::toString( type value )
{
    if ( Fatal == value )
        return "FATAL";
    else if ( Error == value )
        return "ERROR";
    else if ( Warning == value )
        return "WARN";
    else if ( Info == value )
        return "INFO";
    else if ( Debug == value )
        return "DEBUG";
    else if ( Trace == value )
        return "TRACE";
    else if ( Everything == value )
        return "ALL";

    return "OFF";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio

