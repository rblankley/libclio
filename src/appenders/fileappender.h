/**
 * @file fileappender.h
 * @brief File appender class.
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

#ifndef FILEAPPENDER_H
#define FILEAPPENDER_H

#include <appender.h>

#include <fstream>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// File appender class.
/**
 * This appender will output to a file.
 *
 * Properties you may set:
 * @arg file [required] - filename of log
 * @arg appendToFile - true/false value for appending to the file or truncating it
 */
class fileAppender : public appender
{
    typedef fileAppender _Myt;
    typedef appender _Mybase;

public:

    /// File property.
    static constexpr const char *PROP_FILE = "file";

    /// Append to file property.
    static constexpr const char *PROP_APPENDTOFILE = "appendToFile";

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    fileAppender();

    /// Destructor.
    virtual ~fileAppender();

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve file name.
    /**
     * @return  file name
     */
    virtual std::string file() const;

    /// Set file name.
    /**
     * @param[in] value  file name
     */
    virtual void setFile( const std::string& value );

    /// Retrieve if file is appended or truncated.
    /**
     * @return  @c true to append, @c false to truncate
     */
    virtual bool appendToFile() const;

    /// Set if file is appended or truncated.
    /**
     * @param[in] value  @c true to append, @c false to truncate
     */
    virtual void setAppendToFile( bool value );

protected:

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve current write position.
    /**
     * @return  write position
     */
    virtual std::size_t pos() const;

    // ========================================================================
    // Methods
    // ========================================================================

    /// Open the appender.
    /**
     * @return  @c true if opened successfully, @c false otherwise
     */
    virtual bool open();

    /// Close the appender.
    virtual void close();

    /// Write line to appender.
    /**
     * @param[in] line  log line
     */
    virtual void write( const std::string& line );

private:

    std::ofstream ofs_;

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // FILEAPPENDER_H
