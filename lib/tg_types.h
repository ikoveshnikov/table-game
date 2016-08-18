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

#ifndef TG_TYPES_H
#define TG_TYPES_H

#include <cstdint>
#include <utility>
#include <vector>
#include <ostream>

using coordinate_t = std::uint32_t;

struct coordinates_t
{
    coordinate_t x;
    coordinate_t y;

    coordinates_t(coordinate_t kx, coordinate_t ky)
        : x(kx), y(ky) {}
    coordinates_t()
        : x(0), y(0) {}
};

struct wall_coordinates_t
{
    coordinates_t first;
    coordinates_t second;

    wall_coordinates_t(coordinates_t cell1, coordinates_t cell2)
        : first(cell1), second(cell2) {}
    wall_coordinates_t(coordinate_t x1, coordinate_t y1,
                       coordinate_t x2, coordinate_t y2)
        : first(x1,y1), second(x2,y2) {}
};

using ball_id_t = coordinate_t;

using input_data_t = std::vector<coordinate_t>;

#endif // TG_TYPES_H
