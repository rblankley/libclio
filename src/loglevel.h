/**
 * @file loglevel.h
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

#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <string>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Enumeration class of log levels.
class logLevel
{
    typedef logLevel _Myt;

public:

    /// Log levels.
    enum type
    {
        Disabled = 0,                               ///< no logging (not usable from client code)
        Fatal,                                      ///< fatal errors
        Error,                                      ///< bad pointers, exceptions
        Warning,                                    ///< bad configuration, recoverable errors
        Info,                                       ///< configuration, state changes
        Debug,                                      ///< stuff a developer would use to troubleshoot
        Trace,                                      ///< developer stuff but much more detail
        Everything                                  ///< log everything (not usable from client code)
    };

    // ========================================================================
    // Static Methods
    // ========================================================================

    /// Convert from string.
    /**
     * @param[in] value  log level in string form (see above)
     * @return  log level
     */
    static type fromString( const std::string& value );

    /// Convert to string.
    /**
     * @param[in] value  log level
     * @return  log level in string form (see above)
     */
    static std::string toString( type value );

private:

    // not implemented
    logLevel() = delete;

    // not implemented
    logLevel( const _Myt& ) = delete;

    // not implemented
    _Myt& operator = ( const _Myt& ) = delete;

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // LOGLEVEL_H

