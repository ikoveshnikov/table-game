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

#include "table.h"
#include "tg_utils.h"


GameTable::GameTable(const InputData &in)
{
    table_size_ = in.GetTableSize();

    for (coordinate_t i=1; i<table_size_; ++i)
    {
        for (coordinate_t j=1; j<table_size_; ++j)
        {
            board_.insert(std::make_pair(coordinates_t(i,j), BoardCell()));
        }
    }

    // add walls on borders
    for (coordinate_t i=1; i<table_size_; ++i)
    {
        coordinates_t c_up    (i, 1);
        coordinates_t c_down  (i, table_size_);
        coordinates_t c_left  (1, i);
        coordinates_t c_right (table_size_, i);

        board_[c_up].AddWall(Direction::North);
        board_[c_down].AddWall(Direction::South);
        board_[c_left].AddWall(Direction::West);
        board_[c_right].AddWall(Direction::East);
    }

    auto walls = in.GetWalls();
    for (auto i : walls)
    {
        if (i.first.x == i.second.x)
        {
            // vertical neigbours
            if (i.first.y < i.second.y)
            {
                // first is upper than second
                board_[i.first].AddWall(Direction::South);
                board_[i.second].AddWall(Direction::North);
            }
            else
            {
                board_[i.first].AddWall(Direction::North);
                board_[i.second].AddWall(Direction::South);
            }
        }
        else
        {
            // horisontal neighbours
            if (i.first.x < i.second.x)
            {
                // first is left to second
                board_[i.first].AddWall(Direction::East);
                board_[i.second].AddWall(Direction::West);
            }
            else
            {
                board_[i.first].AddWall(Direction::West);
                board_[i.second].AddWall(Direction::East);
            }
        }
    }

    auto holes = in.GetHoles();
    ball_id_t hole_id = 1;
    for (auto i : holes)
    {
        board_[i].AddHole(hole_id);
        ++hole_id;
    }

    auto balls = in.GetBalls();
    ball_id_t ball_id = 1;
    for (auto i : balls)
    {
        balls_.insert(std::make_pair(i, Ball(ball_id)));
        ++ball_id;
    }

}

std::map<const coordinates_t, BoardCell> GameTable::GetBoard() const
{
    return board_;
}

coordinate_t GameTable::GetTableSize() const
{
    return table_size_;
}
