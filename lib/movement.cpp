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

#include "movement.h"
#include "tg_utils.h"

Movement::Movement(Direction to, const Movement &previous_move)
    : move_(to)
    , loop_while_move_north_(false)
    , loop_while_move_west_ (false)
    , loop_while_move_south_(false)
    , loop_while_move_east_ (false)
    , occupied_cells_ (previous_move.GetBallsPositions())
    , holes_state_ (GetHoles())
{

}

Movement::Movement(Direction to,
                   const std::map<coordinates_t, ball_id_t> &balls,
                   const std::map<coordinates_t, ball_id_t> &holes)
    : move_(to)
    , loop_while_move_north_(false)
    , loop_while_move_west_ (false)
    , loop_while_move_south_(false)
    , loop_while_move_east_ (false)
    , occupied_cells_ (balls)
    , holes_state_ (holes)
{

}

void Movement::AddLoop(Direction to)
{
    switch (to)
    {
    case Direction::North:
        loop_while_move_north_ = true;
        break;
    case Direction::West:
        loop_while_move_west_ = true;
        break;
    case Direction::South:
        loop_while_move_south_ = true;
        break;
    case Direction::East:
        loop_while_move_east_ = true;
        break;
    }
}

bool Movement::HasLoops(Direction side) const
{
    switch (side)
    {
    case Direction::North:
        return loop_while_move_north_;
    case Direction::West:
        return loop_while_move_west_;
    case Direction::South:
        return loop_while_move_south_;
    case Direction::East:
        return loop_while_move_east_;
    }

    return false;
}

void Movement::ReqireBallAt(const coordinates_t &cell)
{
    auto search = occupied_cells_.find(cell);

    if (search == occupied_cells_.end())
    {
        // nod found. add to reqired balls
        bool found = false;
        for (auto i : ball_on_way_)
        {
            if (i == cell)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            ball_on_way_.push_back(cell);
        }
    }
    else
    {
        // found - means everything is good, nothing to do
    }
}

void Movement::ReqireClosedHole(const coordinates_t &cell)
{
    auto search = holes_state_.find(cell);

    if (search == holes_state_.end())
    {
        //nothing to do hole is already closed
    }
    else
    {
        // nod found add to reqired balls
        bool found = false;
        for (auto i : holes_closed_)
        {
            if (i == cell)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            holes_closed_.push_back(cell);
        }
    }
}

bool Movement::IsGood() const
{
    return (ball_on_way_.empty() && holes_closed_.empty());
}

bool Movement::SetBallPosition(ball_id_t ball,
                               const coordinates_t & current_cell,
                               const coordinates_t & previous_cell)
{
    auto search = occupied_cells_.find(previous_cell);
    if (search != occupied_cells_.end())
    {
        occupied_cells_.erase(search);
    }

    search = occupied_cells_.find(current_cell);
    if (search != occupied_cells_.end())
    {
        // this cell is already occupied!
        return false;
    }
    else
    {
        // before inserting, check if we have hole under the ball
        auto hole = holes_state_.find(current_cell);
        if (hole != holes_state_.end())
        {
            if (hole->second != ball)
            {
                //wrong hole! illegal move
                return false;
            }
            else
            {
                // dont insert ball into ocupied cells: it closed hole
                // remove ball from reqired list
                RemoveReqiredBall(current_cell);
                return true;
            }
        }
        else
        {
            // safely place ball
            occupied_cells_.insert(std::make_pair(current_cell, ball));
            RemoveReqiredBall(current_cell);
            return true;
        }
    }
}

Direction Movement::GetMove() const
{
    return move_;
}

const std::map<coordinates_t, ball_id_t> &Movement::GetBallsPositions() const
{
    return occupied_cells_;
}

const std::map<coordinates_t, ball_id_t> &Movement::GetHoles() const
{
    return holes_state_;
}

void Movement::RemoveReqiredBall(const coordinates_t &at)
{
    for (auto i = ball_on_way_.begin(); i!= ball_on_way_.end(); ++i)
    {
        if (*i == at)
        {
            ball_on_way_.erase(i);
            return;
        }
    }
}
