/**
 * @file loggermanager.h
 * @brief Logger manager class.
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

#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include "appender.h"
#include "loggermanager.h"
#include "logger.h"

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <memory>
#include <string>
#include <thread>

#if HAVE_FILESYSTEM
#include <filesystem>
#endif

#if HAVE_SHARED_MUTEX
#include <shared_mutex>
#else
#include <mutex>
#endif

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

/// Clio namespace.
namespace clio
{

class loggerManager;

/// Logger manager pointer object.
typedef std::shared_ptr<loggerManager> loggerManagerPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Logger manager class.
class loggerManager
{
    typedef loggerManager _Myt;

public:

    typedef std::chrono::system_clock clock_type;   ///< Clock type refresh interval duration.

    // ========================================================================
    // DTOR
    // ========================================================================

    /// Destructor.
    ~loggerManager();

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve named logger.
    /**
     * @param[in] name  name of logger
     * @return  pointer to logger
     */
    loggerWeakPtr find( const std::string& name ) const;

    /// Retrieve logger refresh interval.
    /**
     * @return  refresh interval
     */
    clock_type::duration refreshInterval() const;

    /// Set logger refresh interval.
    /**
     * Sets how often the configuration file is checked for modifications and reload.
     * @param[in] value  refresh interval
     */
    template <class Rep, class Period>
    void setRefreshInterval( const std::chrono::duration<Rep, Period>& value );

    // ========================================================================
    // Methods
    // ========================================================================

    /// Configure logger based on config file.
    /**
     * @param[in] filename  config filename
     * @return  @c true on success, @c false otherwise
     */
    bool configure( const std::string& filename );

    // ========================================================================
    // Static Methods
    // ========================================================================

    /// Returns pointer to instance.
    /**
     * @return  pointer to @c loggerManager object
     */
    static loggerManagerPtr instance();

    /// Terminate instance.
    static void terminate();

protected:

    // ========================================================================
    // Properties
    // ========================================================================

    /// Set logger refresh interval.
    /**
     * Sets how often the configuration file is checked for modifications and reload.
     * @param[in] interval  refresh interval
     */
    void setRefreshIntervalImpl( const clock_type::duration& interval );

private:

    static constexpr std::chrono::seconds DEFAULT_REFRESH_INTERVAL = std::chrono::seconds( 5 );

#if HAVE_CXX17
    typedef std::shared_mutex mutex;
    typedef std::condition_variable_any condition_variable;
    typedef std::filesystem::file_time_type file_time;
#elif HAVE_CXX14
    typedef std::shared_timed_mutex mutex;
    typedef std::condition_variable_any condition_variable;
    typedef std::time_t file_time;
#else
    typedef std::mutex mutex;
    typedef std::condition_variable condition_variable;
    typedef std::time_t file_time;
#endif

    mutable mutex m_;

    std::string configFile_;
    file_time configFileModifiedTime_;
    std::uintmax_t configFileSize_;

    clock_type::duration refreshInterval_;

    std::thread monitorThread_;
    condition_variable stopMonitoring_;

    appenderPtrMap appenders_;

    loggerPtrMap loggers_;
    loggerPtr rootLogger_;

    static loggerManagerPtr instance_;
    static std::mutex instanceMutex_;

    // ========================================================================

    /// Constructor.
    loggerManager();

    /// Create property from xml.
    template <class T>
    void createProperty( tinyxml2::XMLElement *prop, T *obj );

    /// Create appender from xml.
    void createAppender( tinyxml2::XMLElement *app );

    /// Create logger from xml.
    void createLogger( tinyxml2::XMLElement *log, bool isRoot = false );

    /// Cleanup appenders and loggers.
    void cleanup();

    /// Configure logger based on config file.
    bool setConfiguration( const std::string& filename );

    /// Configure logger based on XML document.
    bool setConfiguration( tinyxml2::XMLDocument *doc );

    /// Monitor for log reconfiguration.
    void monitorConfiguration();

    /// Compare string with wildcards to another string.
    static bool wildCompare( const char *wild, const char *s );

    /// Retrieve file modified time.
    static file_time fileModifiedTime( const std::string& filename );

    /// Retrieve file size.
    static std::uintmax_t fileSize( const std::string& filename );

};

template <class Rep, class Period>
void loggerManager::setRefreshInterval( const std::chrono::duration<Rep, Period>& value )
{
    setRefreshIntervalImpl( value );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // LOGGERMANAGER_H

