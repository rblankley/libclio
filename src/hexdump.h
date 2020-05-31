/**
 * @file hexdump.h
 * @brief Hex dump class.
 *
 * @section Copyright
 * Copyright (C) 2017 Randy Blankley
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

#ifndef HEXDUMP_H
#define HEXDUMP_H

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Hex dump data class.
class hexDumpInfo
{
    typedef hexDumpInfo _Myt;

public:

    static const unsigned int DEFAULT_WIDTH = 16;   ///< Default width when logging hex dumps.

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    /**
     * @param[in] buffer  buffer to log out
     * @param[in] bufferLen  size of buffer
     * @param[in] width  width of hexadecimal lines to write out.
     */
    hexDumpInfo( const void *buffer,
        unsigned int bufferLen,
        unsigned int width = DEFAULT_WIDTH );

    /// Constructor.
    /**
     * @param[in] rhs  object to copy
     */
    hexDumpInfo( const _Myt& rhs );

    /// Destructor.
    virtual ~hexDumpInfo();

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

    /// Retrieve buffer.
    /**
     * @return  buffer
     */
    virtual const void *buffer() const {return buffer_;}

    /// Set buffer.
    /**
     * @param[in] value  buffer
     */
    virtual void setBuffer( const void *value ) {buffer_ = value;}

    /// Retrieve buffer length.
    /**
     * @return  buffer length
     */
    virtual unsigned int bufferLen() const {return bufferLen_;}

    /// Set buffer length.
    /**
     * @param[in] value  buffer length
     */
    virtual void setBufferLen( unsigned int value ) {bufferLen_ = value;}

    /// Retrieve hexadecimal width.
    /**
     * @return  hexadecimal width
     */
    virtual unsigned int width() const {return width_;}

    /// Set hexadecimal width.
    /**
     * @param[in] value  hexadecimal width
     */
    virtual void setWidth( unsigned int value ) {width_ = value;}

protected:

    const void *buffer_;                            ///< Pointer to buffer.
    unsigned int bufferLen_;                        ///< Buffer length.

    unsigned int width_;                            ///< Hexadecimal width.

private:

    /// Copy object.
    void copy( const _Myt& rhs );

};

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // LOGHEXDUMP_H

