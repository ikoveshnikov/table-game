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

#include "board_cell.h"

BoardCell::BoardCell(BoardCell::Walls w)
    : CellObject (CellObject::Type::BoardCell)
    , walls_(w)
{
}

BoardCell::BoardCell()
    : CellObject (CellObject::Type::BoardCell)
    , walls_({false, false, false, false})
    , hole_(false)
    , hole_id_(INVALID_ID)
{
}

BoardCell::~BoardCell()
{

}

void BoardCell::AddWall(Direction at)
{
    switch (at)
    {
    case Direction::East:
        walls_.east = true;
        break;
    case Direction::West:
        walls_.west = true;
        break;
    case Direction::North:
        walls_.north = true;
        break;
    case Direction::South:
        walls_.south = true;
        break;
    }
}

bool BoardCell::HasWall(Direction at) const
{
    switch (at)
    {
    case Direction::East:
        return walls_.east;
    case Direction::West:
        return walls_.west;
    case Direction::North:
        return walls_.north;
    case Direction::South:
        return walls_.south;
    }
    return false;
}

void BoardCell::AddHole(ball_id_t id)
{
    hole_ = true;
}

bool BoardCell::HasHole() const
{
    return hole_;
}

ball_id_t BoardCell::HoleId() const
{
    return hole_id_;
}


