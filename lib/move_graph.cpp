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

#include "move_graph.h"
#include "tg_utils.h"

GraphItem::GraphItem()
    : neighbour_north_({0,0})
    , neighbour_west_ ({0,0})
    , neighbour_south_({0,0})
    , neighbour_east_ ({0,0})
    , invalid_coordinates_({0,0})
{

}

const coordinates_t &GraphItem::GetNeigbour(Direction at) const
{
    switch (at)
    {
    case Direction::North:
        return neighbour_north_;
    case Direction::West:
        return neighbour_west_;
    case Direction::South:
        return neighbour_south_;
    case Direction::East:
        return neighbour_east_;
    }
    return invalid_coordinates_;
}

const std::vector<coordinates_t> &GraphItem::GetHolesOnWayTo(Direction to) const
{
    switch (to)
    {
    case Direction::North:
        return holes_north_;
    case Direction::West:
        return holes_west_;
    case Direction::South:
        return holes_south_;
    case Direction::East:
        return holes_east_;
    }
    return invalid_vector_;
}

void GraphItem::AddNeighbour(Direction at, coordinates_t cell)
{
    switch (at)
    {
    case Direction::North:
        neighbour_north_ = cell;
        break;
    case Direction::West:
        neighbour_west_  = cell;
        break;
    case Direction::South:
        neighbour_south_ = cell;
        break;
    case Direction::East:
        neighbour_east_  = cell;
        break;
    }
}

void GraphItem::AddHole(Direction at, coordinates_t cell)
{
    switch (at)
    {
    case Direction::North:
        holes_north_.push_back(cell);
        break;
    case Direction::West:
        holes_west_.push_back(cell);
        break;
    case Direction::South:
        holes_south_.push_back(cell);
        break;
    case Direction::East:
        holes_east_.push_back(cell);
        break;
    }
}

std::ostream &operator<<(std::ostream &os, const GraphItem &gi)
{
    os << " N: " << gi.GetNeigbour(Direction::North) << " "
       << " W: " << gi.GetNeigbour(Direction::West) << " "
       << " S: " << gi.GetNeigbour(Direction::South) << " "
       << " E: " << gi.GetNeigbour(Direction::East);
    return os;
}
