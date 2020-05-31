/**
 * @file logger.cpp
 * @brief Logger class.
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
#include "logger.h"
#include "logline.h"

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
logger::logger() :
    level_( logLevel::Disabled )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
logger::~logger()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logger::setLevel( logLevel::type value )
{
    if ( value < logLevel::Disabled )
        value = logLevel::Disabled;
    else if ( logLevel::Everything < value )
        value = logLevel::Everything;

    level_ = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
appenderWeakPtrList logger::appenders() const
{
#if HAVE_CXX17 || HAVE_CXX14
    std::shared_lock<mutex> lock( m_ );
#else
    std::lock_guard<mutex> lock( m_ );
#endif

    return appenders_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logger::setAppenders( const appenderWeakPtrList& value )
{
#if HAVE_CXX17 || HAVE_CXX14
    std::unique_lock<mutex> lock( m_ );
#else
    std::lock_guard<mutex> lock( m_ );
#endif

    appenders_ = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool logger::enabled( logLevel::type value ) const
{
    // check log level
    return ( value <= level() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void logger::writeLine( const logLine& line )
{
    // check if enabled
    if ( !enabled( line.level() ) )
        return;

    appenderWeakPtrList apps( appenders() );

    // write to each appender
    for ( const auto& i: apps )
    {
        // lock appender for use
        const auto app( i.lock() );

        if ( app )
            app->writeLine( line );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio

