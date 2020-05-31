/**
 * @file appenderfactory.cpp
 * @brief Appender factory class.
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

#include "appenderfactory.h"

#include "appenders/consoleappender.h"
#include "appenders/fileappender.h"
#include "appenders/rollingfileappender.h"

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
appender *appenderFactory::create( const std::string& type )
{
    if ( "consoleAppender" == type )
        return new consoleAppender();
    else if ( "fileAppender" == type )
        return new fileAppender();
    else if ( "rollingFileAppender" == type )
        return new rollingFileAppender();

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio

