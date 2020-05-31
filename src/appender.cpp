/**
 * @file appender.cpp
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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "appender.h"
#include "layout.h"
#include "logline.h"

/// Clio namespace.
namespace clio
{

///////////////////////////////////////////////////////////////////////////////////////////////////
appender::appender() :
    _Mybase(),
    f_( nullptr )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
appender::~appender()
{
    removeFormatting();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void appender::setFormat( layout *value )
{
    if ( f_ != value )
    {
        removeFormatting();
        f_ = value;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void appender::writeLine( const logLine& line )
{
    std::lock_guard<mutex> guard( m_ );

    // invoke derived class method
    write( f_ ? f_->format( line ) : line.text() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void appender::removeFormatting()
{
    if ( f_ )
    {
        delete f_;
        f_ = nullptr;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace clio



