/**
 * @file propertymap.h
 * @brief Property map class.
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

#ifndef PROPERTYMAP_H
#define PROPERTYMAP_H

#include <map>
#include <sstream>

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////

/// Map of properties.
class propertyMap : private std::map<std::string, std::string>
{
    typedef propertyMap _Myt;
    typedef std::map<std::string, std::string> _Mybase;

public:

    // ========================================================================
    // CTOR / DTOR
    // ========================================================================

    /// Constructor.
    propertyMap() : _Mybase() {}

    /// Destructor.
    virtual ~propertyMap() {}

    // ========================================================================
    // Properties
    // ========================================================================

    /// Retrieve property.
    /**
     * @param[in] name  property name
     * @param[out] found  @c true if property exists, @c false otherwise
     * @return  property value
     */
    template <class T>
    T prop( const std::string& name, bool *found = nullptr ) const;

    /// Set property.
    /**
     * @param[in] name  property name
     * @param[in] value  property value
     */
    template <class T>
    void setProp( const std::string& name, const T& value );

};

template <class T>
T propertyMap::prop( const std::string& name, bool *found ) const
{
    const_iterator i( find( name ) );
    bool exists( end() != i );

    if ( found )
        (*found) = exists;

    if ( !exists )
        return T();

    std::stringstream ios;
    ios << i->second;

    T value;
    ios >> value;

    return value;
}

/// Specialization for string properties.
template <>
inline std::string propertyMap::prop( const std::string& name, bool *found ) const
{
    const_iterator i( find( name ) );
    bool exists( end() != i );

    if ( found )
        (*found) = exists;

    if ( !exists )
        return std::string();

    return i->second;
}

/// Specialization for boolean properties.
template <>
inline bool propertyMap::prop( const std::string& name, bool *found ) const
{
    return ( "true" == prop<std::string>( name, found ) );
}

template <class T>
void propertyMap::setProp( const std::string& name, const T& value )
{
    std::stringstream ios;
    ios << value;

    std::string strValue;
    ios >> strValue;

    iterator i( find( name ) );

    if ( end() == i )
        insert( value_type( name, strValue ) );
    else
    {
        i->second = strValue;
    }
}

/// Specialization for string properties.
template <>
inline void propertyMap::setProp( const std::string& name, const std::string& value )
{
    iterator i( find( name ) );

    if ( end() == i )
        insert( value_type( name, value ) );
    else
    {
        i->second = value;
    }
}

/// Specialization for boolean properties.
template <>
inline void propertyMap::setProp( const std::string& name, const bool& value )
{
    setProp( name, (value ? "true" : "false") );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace clio

#endif // PROPERTYMAP_H
