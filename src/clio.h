/**
 * @file clio.h
 * @brief Main include file for libclio.
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

#ifndef CLIO_H
#define CLIO_H

#include "clioapi.h"
#include "hexdump.h"
#include "logline.h"

#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Initialize clio library.
/**
 * This method may be called multiple times in order to reload configuration.
 * @param[in] filename  config filename
 * @return  @c true on success, @c false otherwise
 */
bool CLIO_API clioInit( const std::string& filename );

/// Retrieve refresh interval.
/**
 * Retrieve interval between configuration refreshes. The clio library will periodically check the
 * configuration file for changes and reload when this file changes.
 * @return  interval period in milliseconds
 */
unsigned int CLIO_API clioGetRefeshInterval();

/// Set refresh interval.
/**
 * Set interval between configuration refreshes. The clio library will periodically check the
 * configuration file for changes and reload when this file changes.
 * @param[in] interval_ms  interval period in milliseconds
 */
void CLIO_API clioSetRefeshInterval( unsigned int interval_ms );

/// Retrieve version of clio library.
/**
 * @return  version string
 */
std::string CLIO_API clioGetVersion();

/// Finalize clio library.
void CLIO_API clioFinalize();

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Initialize clio library with config file.
/**
 * @code
 * CLIO_INIT( "myapp.config" );
 * @endcode
 */
#define CLIO_INIT( CONFIGFILE ) \
    clioInit( CONFIGFILE )

/// Initialize clio library with config file and refresh interval (in milliseconds).
/**
 * @code
 * CLIO_INIT( "myapp.config", 5000 );
 * @endcode
 */
#define CLIO_INIT_WITH_INTERVAL( CONFIGFILE, INTERVAL ) \
    clioSetRefeshInterval( INTERVAL ); \
    clioInit( CONFIGFILE )

/// Finalize clio library.
/**
 * @code
 * CLIO_FINALIZE
 * @endcode
 */
#define CLIO_FINALIZE \
    clioFinalize();

/// Log fatal message.
/**
 * @code
 * LOG_FATAL << "this is a fatal error!";
 * @endcode
 */
#define LOG_FATAL \
    clio::logLine( clio::logLevel::Fatal, __FILE__, __PRETTY_FUNCTION__, __LINE__ )

/// Log error message.
/**
 * @code
 * LOG_ERROR << "this is an error!";
 * @endcode
 */
#define LOG_ERROR \
    clio::logLine( clio::logLevel::Error, __FILE__, __PRETTY_FUNCTION__, __LINE__ )

/// Log warning message.
/**
 * @code
 * LOG_WARN << "this is a warning!";
 * @endcode
 */
#define LOG_WARN \
    clio::logLine( clio::logLevel::Warning, __FILE__, __PRETTY_FUNCTION__, __LINE__ )

/// Log informational message.
/**
 * @code
 * LOG_INFO << "this is informational";
 * @endcode
 */
#define LOG_INFO \
    clio::logLine( clio::logLevel::Info, __FILE__, __PRETTY_FUNCTION__, __LINE__ )

/// Log debug message.
/**
 * @code
 * LOG_DEBUG << "this is a debug message!";
 * @endcode
 */
#define LOG_DEBUG \
    clio::logLine( clio::logLevel::Debug, __FILE__, __PRETTY_FUNCTION__, __LINE__ )

/// Log trace message.
/**
 * @code
 * LOG_TRACE << "this is a trace message!";
 * @endcode
 */
#define LOG_TRACE \
    clio::logLine( clio::logLevel::Trace, __FILE__, __PRETTY_FUNCTION__, __LINE__ )

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Log hex dump with default width.
/**
 * @code
 * LOG_TRACE << HEX_DUMP( buffer, bufferLen );
 * @endcode
 */
#define HEX_DUMP( buffer, bufferLen ) \
    clio::hexDumpInfo( buffer, bufferLen )

/// Log hex dump.
/**
 * @code
 * LOG_TRACE << HEX_DUMP_W( buffer, bufferLen, width );
 * @endcode
 */
#define HEX_DUMP_W( buffer, bufferLen, width ) \
    clio::hexDumpInfo( buffer, bufferLen, width )

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // CLIO_H
