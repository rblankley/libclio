/**
 * @file patternlayout.h
 * @brief Pattern layout class.
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

#ifndef PATTERNLAYOUT_H
#define PATTERNLAYOUT_H

#include "../layout.h"

#include <list>
#include <string>

/// Clio namespace.
namespace clio
{

class logLine;

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Pattern layout class.
/**
 * This layout class will format log lines using a pre-defined pattern. You may control the pattern
 * by setting the property "conversionPattern".
 *
 * List of possible values:
 * @arg %date - current date/time
 * @arg %epoch - current date/time in unix epoch format
 * @arg %epochms - current date/time in unix epoch format (milliseconds)
 * @arg %thread - thread number
 * @arg %levelnum - log level in numeric form
 * @arg %level - log level in string form
 * @arg %module - module name
 * @arg %class - class name
 * @arg %method - class method
 * @arg %message - logged message
 * @arg %file - source file name
 * @arg %linenum - source file line number
 * @arg %newline - new line
 *
 * Some example formats:
 * %date{%m/%d/%Y %H:%M:%S.%L} [%thread{%08x},%levelnum] %class{%-15.15s} %method{%-15.15s} %message%newline
 * %date{%Y%m%d %H%M%S%L} %level{%-5.5s} %message
 * %message (%module,%file,%linenum)%newline
 */
class patternLayout : public layout
{
    typedef patternLayout _Myt;
    typedef layout _Mybase;

public:

    /// Conversion pattern property.
    static constexpr const char *PROP_CONVERSIONPATTERN = "conversionPattern";

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    patternLayout();

    /// Destructor.
    virtual ~patternLayout();

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve conversion pattern.
    /**
     * @return  conversion pattern
     */
    virtual std::string conversionPattern() const;

    /// Set conversion pattern.
    /**
     * @param[in] value  conversion pattern
     */
    virtual void setConversionPattern( const std::string& value );

    // ========================================================================
    // Methods
    // ========================================================================

    /// Format a log line into string form suitable for output.
    /**
     * @param[in] line  log line
     * @return  formatted log line
     */
    virtual std::string format( const logLine& line ) const;

private:

    static constexpr const char *DEFAULT_DATE_FORMAT = "%m/%d/%Y %H:%M:%S.%L";
    static constexpr const char *DEFAULT_LARGE_NUMBER_FORMAT = "%lld";
    static constexpr const char *DEFAULT_NUMBER_FORMAT = "%d";
    static constexpr const char *DEFAULT_STRING_FORMAT = "%s";

    typedef std::list<std::string> stringList;
    stringList formats_;

    // ========================================================================

    /// Find format in string.
    bool findFormat( const std::string& line, std::string& name, std::string& format ) const;

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // PATTERNLAYOUT_H
