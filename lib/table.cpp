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

#include <iomanip>

#include "tg_utils.h"


GameTable::GameTable(const InputData &in)
{
    table_size_ = in.GetTableSize();

    for (coordinate_t i=1; i<=table_size_; ++i)
    {
        for (coordinate_t j=1; j<table_size_; ++j)
        {
            board_.insert(std::make_pair(coordinates_t(i,j), BoardCell()));
        }
    }

    // add walls on borders
    for (coordinate_t i=1; i<=table_size_; ++i)
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

void GameTable::CalculateMoves()
{
    BuildMoveGraph();
}

void GameTable::BuildMoveGraph()
{
    //TODO: run this code in parallel, using OpenMP or Intel TBB
    for (auto i : board_)
    {
        coordinates_t cell = i.first;
        GraphItem gi;

        FillGraphItemInDirection(gi, cell, Direction::North);
        FillGraphItemInDirection(gi, cell, Direction::West);
        FillGraphItemInDirection(gi, cell, Direction::South);
        FillGraphItemInDirection(gi, cell, Direction::East);

        move_graph_.insert(std::make_pair(cell, gi));
    }
}

std::pair<Ball::CollisionResult, coordinates_t>
GameTable::RollBall(const coordinates_t &start_from,
                    const Direction to) const
{
    coordinates_t current_cell = start_from;
    Ball::CollisionResult collision = Ball::CollisionResult::Pass;
    Ball ball (INVALID_ID);

    while ( (collision = ball.CollisionWith(board_.at(current_cell), to)) ==
            Ball::CollisionResult::Pass )
    {
        switch (to)
        {
        case Direction::North:
            current_cell.x -= 1;
            break;
        case Direction::West:
            current_cell.y -= 1;
            break;
        case Direction::South:
            current_cell.x += 1;
            break;
        case Direction::East:
            current_cell.y += 1;
            break;
        }
    }

    return std::make_pair(collision, current_cell);
}

void GameTable::FillGraphItemInDirection(GraphItem &gi,
                                         coordinates_t current_cell,
                                         Direction move_to) const
{
    Ball::CollisionResult collision = Ball::CollisionResult::Pass;
    coordinates_t start_cell = current_cell;
    coordinates_t collision_cell;
    do {
        auto c_result = RollBall(start_cell, move_to);
        collision = c_result.first;
        collision_cell = c_result.second;
        switch (collision)
        {
        case Ball::CollisionResult::Pass:
            break;
        case Ball::CollisionResult::Stop:
            gi.AddNeighbour(move_to, collision_cell);
            break;
        case Ball::CollisionResult::FallToHoleOk:
        case Ball::CollisionResult::FallToHoleFail:
            gi.AddHole(move_to, collision_cell);
            break;
        }
        start_cell = collision_cell;

    } while (collision != Ball::CollisionResult::Stop);
}

std::ostream &
operator << (std::ostream & os, const GameTable & gt)
{
    auto board = gt.GetBoard();
    auto table_size = gt.GetTableSize();

    for (coordinate_t j=1; j<=table_size; ++j)
    {
        for (coordinate_t i=1; i<=table_size; ++i)
        {
            os << ". . . .";
        }
        os << ".\n";

        for (coordinate_t i=1; i<=table_size; ++i)
        {
            os << ".";
            if (board.at(coordinates_t(i,j)).HasWall(Direction::North) &&
                board.at(coordinates_t(i,j)).HasWall(Direction::West))
                os << "┌";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::North))
                    os << "─";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::West))
                    os << "│";
            else
                os << " ";

            if (board.at(coordinates_t(i,j)).HasWall(Direction::North))
                os << "────";
            else
                os << "    ";

            if (board.at(coordinates_t(i,j)).HasWall(Direction::North) &&
                board.at(coordinates_t(i,j)).HasWall(Direction::East))
                os << "┐";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::North))
                    os << "─";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::East))
                    os << "│";
            else
                os << " ";
        }
        os << ".\n";

        for (coordinate_t i=1; i<=table_size; ++i)
        {
            os << ".";
            if (board.at(coordinates_t(i,j)).HasWall(Direction::West))
                os << "│";
            else
                os << " ";
            if (board.at(coordinates_t(i,j)).HasHole())
                os << "H " << std::setw(2) << board.at(coordinates_t(i,j)).HoleId();
            else
                os << "    ";
            if (board.at(coordinates_t(i,j)).HasWall(Direction::East))
                os << "│";
            else
                os << " ";

        }
        os << ".\n";

        for (coordinate_t i=1; i<=table_size; ++i)
        {
            os << ".";
            if (board.at(coordinates_t(i,j)).HasWall(Direction::South) &&
                board.at(coordinates_t(i,j)).HasWall(Direction::West))
                os << "└";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::South))
                    os << "─";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::West))
                    os << "│";
            else
                os << " ";

            if (board.at(coordinates_t(i,j)).HasWall(Direction::South))
                os << "────";
            else
                os << "    ";

            if (board.at(coordinates_t(i,j)).HasWall(Direction::South) &&
                board.at(coordinates_t(i,j)).HasWall(Direction::East))
                os << "┘";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::South))
                    os << "─";
            else if (board.at(coordinates_t(i,j)).HasWall(Direction::East))
                    os << "│";
            else
                os << " ";
        }
        os << ".\n";
    }
    for (coordinate_t j=1; j<=table_size; ++j)
    {
        os << ". . . .";
    }
    os << ".\n";

    return os;
}
