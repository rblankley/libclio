/**
 * @file layout.h
 * @brief Base layout class.
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

#ifndef LAYOUT_H
#define LAYOUT_H

#include "propertymap.h"

#include <string>

/// Clio namespace.
namespace clio
{

class logLine;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Layout base class.
class layout : public propertyMap
{
    typedef layout _Myt;
    typedef propertyMap _Mybase;

public:

    // ========================================================================
    // DTOR
    // ========================================================================

    /// Destructor.
    virtual ~layout();

    // ========================================================================
    // Methods
    // ========================================================================

    /// Format a log line into string form suitable for output.
    /**
     * @param[in] line  log line
     * @return  formatted log line
     */
    virtual std::string format( const logLine& line ) const;

protected:

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    layout();

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // LAYOUT_H

