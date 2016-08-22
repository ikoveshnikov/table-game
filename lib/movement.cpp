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
    , start_move_(false)
    , occupied_cells_ (previous_move.GetBallsPositions())
    , holes_state_ (previous_move.GetHoles())
    , loop_guard_(previous_move.GetLoopGuard())
{

}

Movement::Movement(const std::map<coordinates_t, ball_id_t> &balls,
                   const std::map<coordinates_t, ball_id_t> &holes)
    : start_move_ (true)
    , occupied_cells_ (balls)
    , holes_state_ (holes)
{
    for (auto ball : balls)
    {
        LoopGuard lg;
        lg.visited_cells_[ball.first] = true;

        loop_guard_.insert(std::make_pair(ball.second, lg));
    }
}

bool Movement::IsStartMove() const
{
    return start_move_;
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
            if (hole->second == ball)
            {
                holes_state_.erase(hole);
                return true;
            }
        }
        else
        {
            // just place ball on the cell
            occupied_cells_.insert(std::make_pair(current_cell, ball));
            loop_guard_.at(ball).visited_cells_[current_cell] = true;
            return true;
        }
    }
    return false;
}

const coordinates_t Movement::GetBallPosition(ball_id_t ball)
{
    for (auto i : occupied_cells_)
    {
        if (i.second == ball)
        {
            return i.first;
        }
    }
    return coordinates_t(0,0);
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

const std::map<ball_id_t, Movement::LoopGuard> Movement::GetLoopGuard() const
{
    return loop_guard_;
}

void Movement::InitLoopGuard()
{
    for (auto lg : loop_guard_)
    {
        lg.second.visited_cells_count_ = lg.second.visited_cells_.size();
    }
}

bool Movement::IsLooped() const
{
    for (auto lg : loop_guard_)
    {
        if (lg.second.visited_cells_count_ != lg.second.visited_cells_.size())
        {
            return false;
        }
    }
    return true;
}
