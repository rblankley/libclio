/**
 * @file consoleappender.h
 * @brief Console appender class.
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

#ifndef CONSOLEAPPENDER_H
#define CONSOLEAPPENDER_H

#include <appender.h>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Console appender class.
/**
 * This appender will output to the console.
 */
class consoleAppender : public appender
{
    typedef consoleAppender _Myt;
    typedef appender _Mybase;

public:

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    consoleAppender();

    /// Destructor.
    virtual ~consoleAppender();

protected:

    // ========================================================================
    // Methods
    // ========================================================================

    /// Write line to appender.
    /**
     * @param[in] line  log line
     */
    virtual void write( const std::string& line );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // CONSOLEAPPENDER_H
