/*
 * Copyright (c) 2016, Ivan Koveshnikov
 * ikoveshnik@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of ofp-pfe nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TG_UTILS_H
#define TG_UTILS_H

#include "tg_types.h"
#include <set>
#include <cstring>

inline bool
operator== (const coordinates_t & l, const coordinates_t & r)
{
    return ((l.x == r.x) && (l.y == r.y));
}

inline bool
operator!= (const coordinates_t & l, const coordinates_t & r)
{
    return ((l.x != r.x) && (l.y != r.y));
}

inline bool
operator< (const coordinates_t & l, const coordinates_t & r)
{
    return (std::memcmp(&l, &r, sizeof(coordinates_t)) < 0);
}

//!
//! \brief operator == Compare two wall coordinates.
//! Since each wall coordinate is represented by two neighbours cells
//! need to cross compare all cells
//! \param l coordinates of first wall
//! \param r coorinates of second wall
//! \return true if equal
//!
inline bool
operator== (const wall_coordinates_t & l, const wall_coordinates_t & r)
{
    // No matter which of cell is first and which is second one
    // border is BETWEEN them
    return ((l.first  == r.first)  && (l.second == r.second)) ||
            ((l.first == r.second) && (l.second == r.first));
}

//!
//! \brief operator != same as operator compare
//! \see %operator==
//! \param l coordinates of first wall
//! \param r coorinates of second wall
//! \return true if NOT equal
//!
inline bool
operator!= (const wall_coordinates_t & l, const wall_coordinates_t & r)
{
    // No matter which of cell is first and which is second one
    // border is BETWEEN them
    return ((l.first  != r.first)  && (l.second != r.second)) ||
            ((l.first != r.second) && (l.second != r.first));
}

inline std::ostream &
operator<<(std::ostream & os, const coordinates_t & c)
{
    os << "( " << c.x << " , " << c.y << " )";
    return os;
}

inline std::ostream &
operator<<(std::ostream & os, const wall_coordinates_t & c)
{
    os << "( " << c.first << " , " << c.second << " )";
    return os;
}

//!
//! \brief IsValid Check if coordinate valid: more or equal 1 and less or equal
//! game board size
//! \param c coordinate
//! \param table_size game board size
//! \return true if valid
//!
inline bool IsValid(const coordinate_t c, const coordinate_t table_size)
{
    return ((c >= 1) && (c <= table_size));
}

//!
//! \brief IsValid Check if coordinate valid: must lay on game board
//! \param c cells coordinate
//! \param table_size size of game board
//! \return true if valid
//!
inline bool IsValid(const coordinates_t &c, const coordinate_t table_size)
{
    return ( IsValid(c.x, table_size) &&
             IsValid(c.y, table_size) );
}

inline bool IsValid(const wall_coordinates_t &c, const coordinate_t table_size)
{
    return ( IsValid(c.first, table_size) &&
             IsValid(c.second, table_size) );
}

//!
//! /brief Find duplicated items in vector
//!
template < typename T >
inline bool HasDuplicates (const std::vector<T> & v)
{
    for(size_t i = 0; i < v.size() - 1; ++i)
    {
        for(size_t j = i+1; j < v.size(); ++j)
        {
            if (v[i] == v[j])
            {
                return true;
            }
        }
    }
    return false;
}

#endif // TG_UTILS_H
