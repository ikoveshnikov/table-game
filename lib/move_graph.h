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

#ifndef TG_MOVE_GRAPH_H
#define TG_MOVE_GRAPH_H

#include "tg_types.h"
#include <vector>
#include <ostream>

class GraphItem
{
public:
    GraphItem();
    ~GraphItem() = default;

    const coordinates_t & GetNeigbour(Direction at) const;
    const std::vector <coordinates_t> & GetHolesOnWayTo (Direction to) const;

    void AddNeighbour (Direction at, coordinates_t cell);
    void AddHole      (Direction at, coordinates_t cell);

private:
    //direct connection
    coordinates_t neighbour_north_;
    coordinates_t neighbour_west_;
    coordinates_t neighbour_south_;
    coordinates_t neighbour_east_;

    //holes on way to connections
    std::vector<coordinates_t> holes_north_;
    std::vector<coordinates_t> holes_west_;
    std::vector<coordinates_t> holes_south_;
    std::vector<coordinates_t> holes_east_;

    const coordinates_t invalid_coordinates_;
    const std::vector<coordinates_t> invalid_vector_;
};

std::ostream &
operator<< (std::ostream & os, const GraphItem & gi);

#endif //TG_MOVE_GRAPH_H
