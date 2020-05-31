/**
 * @file logline.h
 * @brief Log line class.
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

#ifndef LOGLINE_H
#define LOGLINE_H

#include "hexdump.h"
#include "logger.h"
#include "loglevel.h"

#include <chrono>
#include <cstdarg>
#include <string>
#include <sstream>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Log line data class.
class logLine
{
    typedef logLine _Myt;

public:

    typedef std::chrono::system_clock clock_type;   ///< Clock type for time stamps.

    static const unsigned int DEFAULT_WIDTH = 16;   ///< Default width when logging hex dumps.

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    /**
     * @param[in] level  log line level
     * @param[in] file  source file name (use the __FILE__ macro)
     * @param[in] function  source file name (use the __PRETTY_FUNCTION__ macro)
     * @param[in] line  source file line (use the __LINE__ macro)
     */
    logLine( logLevel::type level,
        const char *file,
        const char *function,
        unsigned int line );

    /// Constructor.
    /**
     * @param[in] rhs  object to copy
     */
    logLine( const _Myt& rhs );

    /// Destructor.
    virtual ~logLine();

    // ========================================================================
    // Operators
    // ========================================================================

    /// Assignment operator.
    /**
     * @param[in] rhs  value to assign from
     * @return  reference to @c this
     */
    _Myt& operator = ( const _Myt& rhs ) {copy( rhs ); return *this;}

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
    virtual void setLevel( logLevel::type value ) {level_ = value;}

    /// Retrieve module name.
    /**
     * @return  module/library name
     */
    virtual std::string moduleName() const {return moduleName_;}

    /// Set module name.
    /**
     * @param[in] value  module/library name
     */
    virtual void setModuleName( const std::string& value ) {moduleName_ = value;}

    /// Retrieve class name.
    /**
     * @return  source file class name
     */
    virtual std::string className() const {return className_;}

    /// Set class name.
    /**
     * @param[in] value  class name
     */
    virtual void setClassName( const std::string& value ) {className_ = value;}

    /// Retrieve class function.
    /**
     * @return  source file class function name
     */
    virtual std::string classFunction() const {return classFunction_;}

    /// Set class function name.
    /**
     * @param[in] value  class function name
     */
    virtual void setClassFunction( const std::string& value ) {classFunction_ = value;}

    /// Retrieve source filename.
    /**
     * @return  source file name
     */
    virtual std::string sourceFilename() const {return sourceFilename_;}

    /// Set source filename.
    /**
     * Best way to set this is to use the @c __FILE__ macro.
     *
     * @param[in] value  source file name
     */
    virtual void setSourceFilename( const std::string& value ) {sourceFilename_ = value;}

    /// Retrieve source file line number.
    /**
     * @return  source file line
     */
    virtual unsigned int sourceLine() const {return sourceLine_;}

    /// Set source file line number
    /**
     * Best way to set this is to use the @c __LINE__ macro.
     *
     * @param[in] value  source file line
     */
    virtual void setSourceLine( unsigned int value ) {sourceLine_ = value;}

    /// Retrieve log text line.
    /**
     * @return  log text
     */
    virtual std::string text() const {return text_;}

    /// Set log text.
    /**
     * @param[in] value  log text
     */
    virtual void setText( const std::string& value ) {text_ = value;}

    /// Set log text from format.
    /**
     * This function will set text from a "C" style formatted string.
     *
     * @param[in] format  format string
     */
    virtual void setText( const char *format, ... );

    /// Set log text to hexadecimal string from buffer.
    /**
     * This function will write a buffer in hexadecimal to the log text.
     *
     * The @p width parameter will control how big the text line is.  For example, using a width
     * of 8 on a buffer of numbers 0-19 would look like:
     * 00000000  00 01 02 03 04 05 06 07 08  ........
     * 00000008  09 0A 0B 0C 0D 0E 0F 10 11  ........
     * 00000010  12 13 14 15                 ....
     *
     * @param[in] buffer  buffer to log out
     * @param[in] bufferLen  size of buffer
     * @param[in] width  width of hexadecimal lines to write out.
     */
    virtual void setTextHex( const void *buffer, unsigned int bufferLen, unsigned int width = DEFAULT_WIDTH );

    /// Retrieve log line time stamp.
    /**
     * @return  time stamp of log line
     */
    virtual clock_type::time_point timeStamp() const {return stamp_;}

    /// Set log line time stamp.
    /**
     * @param[in] value  time stamp
     */
    template <class Clock, class Duration>
    void setTimeStamp( const std::chrono::time_point<Clock, Duration>& value );

    /// Get log line thread id.
    /**
     * This is the thread id of who logged the line.
     *
     * @return  thread id
     */
    virtual std::size_t threadId() const {return threadId_;}

    /// Set thread id.
    /**
     * @param[in] value  thread id
     */
    virtual void setThreadId( std::size_t value ) {threadId_ = value;}

    /// Check if log enabled.
    /**
     * @return  @c true if enabled, @c false otherwise
     */
    virtual bool enabled() const;

    // ========================================================================
    // Methods
    // ========================================================================

    /// Append log text.
    /**
     * @param[in] value  log text
     */
    virtual void appendText( const std::string& value ) {text_.append( value );}

    /// Append log text from format.
    /**
     * This function will append text from a "C" style formatted string.
     *
     * @param[in] format  format string
     */
    virtual void appendText( const char *format, ... );

    /// Append to the log a hexadecimal string from buffer.
    /**
     * This function will write a buffer in hexadecimal to the log text.
     *
     * The @p width parameter will control how big the text line is.  For example, using a width
     * of 8 on a buffer of numbers 0-19 would look like:
     * 00000000  00 01 02 03 04 05 06 07 08  ........
     * 00000008  09 0A 0B 0C 0D 0E 0F 10 11  ........
     * 00000010  12 13 14 15                 ....
     *
     * @param[in] buffer  buffer to log out
     * @param[in] bufferLen  size of buffer
     * @param[in] width  width of hexadecimal lines to write out.
     */
    virtual void appendTextHex( const void *buffer, unsigned int bufferLen, unsigned int width = DEFAULT_WIDTH );

protected:

    // ========================================================================
    // Properties
    // ========================================================================

    /// Get logger name from @c logLine properties.
     /**
     * @return  logger name
     */
    virtual std::string loggerName() const;

    /// Set logger name from function text (i.e. __PRETTY_FUNCTION__ macro).
    /**
     * @param[in] function  function text
     */
    virtual void setLoggerName( const char *function );

    /// Set log line time stamp.
    /**
     * @param[in] value  time stamp
     */
    virtual void setTimeStampImpl( const clock_type::time_point& value ) {stamp_ = value;}

private:

    static const unsigned int MIN_WIDTH = 8;
    static const unsigned int MAX_WIDTH = 64;

    logLevel::type level_;

    std::string moduleName_;
    std::string className_;
    std::string classFunction_;

    std::string sourceFilename_;
    unsigned int sourceLine_;

    std::string text_;

    clock_type::time_point stamp_;
    std::size_t threadId_;

    loggerWeakPtr logger_;

    // ========================================================================

    /// Copy object.
    void copy( const _Myt& rhs );

    /// Append text.
    void appendTextV( const char *format, std::va_list params );

};

template <class Clock, class Duration>
void logLine::setTimeStamp( const std::chrono::time_point<Clock, Duration>& value )
{
    setTimeStampImpl( value );
}

/// Stream insertion operator.
/**
 * @param[in,out] lhs  @c logLine to insert into
 * @param[in] rhs  value to insert
 * @return  reference to the @c logLine object
 */
template <class T>
logLine& operator << ( logLine& lhs, const T& rhs )
{
    if ( lhs.enabled() )
    {
        std::stringstream os;
        os << rhs;

        lhs.appendText( os.str() );
    }

    return lhs;
}

/// Specialization for strings.
template <>
inline logLine& operator << ( logLine& lhs, const std::string& rhs )
{
    if ( lhs.enabled() )
        lhs.appendText( rhs );

    return lhs;
}

/// Specialization for hex dumps.
template <>
inline logLine& operator << ( logLine& lhs, const hexDumpInfo& rhs )
{
    if ( lhs.enabled() )
        lhs.appendTextHex( rhs.buffer(), rhs.bufferLen(), rhs.width() );

    return lhs;
}

/// Stream insertion operator.
/**
 * @param[in,out] lhs  rvalue reference of @c logLine to insert into
 * @param[in] rhs  value to insert
 * @return  modified @c logLine object
 */
template <class T>
logLine& operator << ( logLine&& lhs, const T& rhs )
{
    return (lhs << rhs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // LOGLINE_H

