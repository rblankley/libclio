/**
 * @file rollingfileappender.h
 * @brief File appender that rolls when reaching specified size.
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

#ifndef ROLLINGFILEAPPENDER_H
#define ROLLINGFILEAPPENDER_H

#include "fileappender.h"

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Rolling file appender class.
/**
 * This file appender will roll over upon reaching a size.
 *
 * Properties you may set:
 * @arg maxSizeRollBackups - how many old logs too keep
 * @arg maximumFileSize - how large a file can get before rolling (in MB)
 */
class rollingFileAppender : public fileAppender
{
    typedef rollingFileAppender _Myt;
    typedef fileAppender _Mybase;

public:

    /// How many rolled backups to keep.
    static constexpr const char *PROP_MAXSIZEROLLBACKUPS = "maxSizeRollBackups";

    /// Maximum file size.
    static constexpr const char *PROP_MAXIMUMFILESIZE = "maximumFileSize";

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    rollingFileAppender();

    /// Destructor.
    virtual ~rollingFileAppender();

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve how many rolled backups to keep.
    /**
     * @return  rolled backup count
     */
    virtual std::size_t maxSizeRollBackups() const;

    /// Set how many rolled backups to keep.
    /**
     * @param[in] value  rolled backup count
     */
    virtual void setMaxSizeRollBackups( std::size_t value );

    /// Retrieve maximum file size.
    /**
     * @return  maximum file size (in MB)
     */
    virtual std::size_t maximumFileSize() const;

    /// Set maximum file size.
    /**
     * @param[in] value  maximum file size (in MB)
     */
    virtual void setMaximumFileSize( std::size_t value );

protected:

    // ========================================================================
    // Methods
    // ========================================================================

    /// Write line to appender.
    /**
     * @param[in] line  log line
     */
    virtual void write( const std::string& line );

private:

    /// Check if we need to roll logs.
    bool shouldRollLogs() const;

    /// Roll logs over.
    void rollLogs();

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // ROLLINGFILEAPPENDER_H
