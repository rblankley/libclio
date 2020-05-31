/**
 * @file appenderfactory.h
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

#ifndef APPENDERFACTORY_H
#define APPENDERFACTORY_H

#include <string>

/// Clio namespace.
namespace clio
{

class appender;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Appender factory class.
class appenderFactory
{
    typedef appenderFactory _Myt;

public:

    // ========================================================================
    // Static Methods
    // ========================================================================

    /// Factory method for creating appenders.
    /**
     * @param[in] type  appender type
     * @return  pointer to appender, or NULL on failure
     */
    static appender *create( const std::string& type );

private:

    // not implemented
    appenderFactory() = delete;

    // not implemented
    appenderFactory( const _Myt& ) = delete;

    // not implemented
    _Myt& operator = ( const _Myt& ) = delete;

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio


#endif // APPENDERFACTORY_H
