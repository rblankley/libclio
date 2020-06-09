/**
 * @file clio.cpp
 * @brief Library methods for libclio.
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

#include "clio.h"
#include "loggermanager.h"

#if _WIN32
static constexpr const char *PACKAGE_VERSION = "1.0.0";
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
bool clioInit( const std::string& filename )
{
    return clio::loggerManager::instance()->configure( filename );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int clioGetRefeshInterval()
{
    const std::chrono::milliseconds interval(
        std::chrono::duration_cast<std::chrono::milliseconds>( clio::loggerManager::instance()->refreshInterval() ) );

    return (unsigned int) interval.count();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void clioSetRefeshInterval( unsigned int interval_ms )
{
    clio::loggerManager::instance()->setRefreshInterval( std::chrono::milliseconds( interval_ms ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
std::string clioGetVersion()
{
    return std::string( PACKAGE_VERSION );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void clioFinalize()
{
    return clio::loggerManager::instance()->terminate();
}
