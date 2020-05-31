/**
 * @file appender.h
 * @brief Base appender class.
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

#ifndef APPENDER_H
#define APPENDER_H

#include "propertymap.h"

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>

/// Clio namespace.
namespace clio
{

class layout;
class logLine;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Appender base class.
class appender : public propertyMap
{
    friend class loggerManager;

    typedef appender _Myt;
    typedef propertyMap _Mybase;

public:

    // ========================================================================
    // DTOR
    // ========================================================================

    /// Destructor.
    virtual ~appender();

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve appender format.
    /**
     * @return  pointer to layout object
     */
    virtual layout *format() const {return f_;}

    /// Set appender format.
    /**
     * @param[in] value  pointer to layout object
     */
    virtual void setFormat( layout *value );

    // ========================================================================
    // Methods
    // ========================================================================

    /// Write the log line to the appender.
    /**
     * @param[in] line  log line
     */
    virtual void writeLine( const logLine& line );

protected:

    // ========================================================================
    // CTOR
    // ========================================================================

    /// Constructor.
    appender();

    // ========================================================================
    // Methods
    // ========================================================================

    /// Open the appender.
    /**
     * @return  @c true if opened successfully, @c false otherwise
     */
    virtual bool open() {return true;}

    /// Close the appender.
    virtual void close() {}

    /// Write line to appender.
    /**
     * @param[in] line  log line
     */
    virtual void write( const std::string& line ) = 0;

private:

    typedef std::mutex mutex;
    mutable mutex m_;

    layout *f_;

    // ========================================================================

    /// Remove appender formatting.
    void removeFormatting();

};

/// Appender pointer object.
typedef std::shared_ptr<appender> appenderPtr;

/// Map of appender pointer objects by appender name.
typedef std::map<std::string, appenderPtr> appenderPtrMap;

/// Appender weak pointer object.
typedef std::weak_ptr<appender> appenderWeakPtr;

/// List of appender weak pointer objects.
typedef std::list<appenderWeakPtr> appenderWeakPtrList;

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio


#endif // APPENDER_H
