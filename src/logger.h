/**
 * @file logger.h
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

#ifndef LOGGER_H
#define LOGGER_H

#include "appender.h"
#include "loglevel.h"

#include <map>
#include <memory>

#if HAVE_SHARED_MUTEX
#include <shared_mutex>
#else
#include <mutex>
#endif

/// Clio namespace.
namespace clio
{

class logLine;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Logger class.
class logger
{
    typedef logger _Myt;

public:

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    logger();

    /// Destructor.
    virtual ~logger();

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve log level.
    /**
     * @return  log level
     */
    virtual logLevel::type level() const {return level_;}

    /// Set log level.
    /**
     * @param[in] value  log level
     */
    virtual void setLevel( logLevel::type value );

    /// Retrieve appender list.
    /**
     * @return  appender list
     */
    virtual appenderWeakPtrList appenders() const;

    /// Set appender list.
    /**
     * @param[in] value  appender list
     */
    virtual void setAppenders( const appenderWeakPtrList& value );

    /// Check if log enabled for logLevel @p value.
    /**
     * @param[in] value  log level
     * @return  @c true if enabled, @c false otherwise
     */
    virtual bool enabled( logLevel::type value ) const;

    // ========================================================================
    // Methods
    // ========================================================================

    /// Write the log line to the logger.
    /**
     * @param[in] line  log line
     */
    virtual void writeLine( const logLine& line );

private:

#if HAVE_CXX17
    typedef std::shared_mutex mutex;
#elif HAVE_CXX14
    typedef std::shared_timed_mutex mutex;
#else
    typedef std::mutex mutex;
#endif

    mutable mutex m_;

    logLevel::type level_;

    appenderWeakPtrList appenders_;

};

/// Logger pointer object.
typedef std::shared_ptr<logger> loggerPtr;

/// Map of logger pointer objects by logger name.
typedef std::map<std::string, loggerPtr> loggerPtrMap;

/// Logger weak pointer object.
typedef std::weak_ptr<logger> loggerWeakPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // LOGGER_H

