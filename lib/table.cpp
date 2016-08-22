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
#include <cassert>

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
        holes_[hole_id] = i;
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
    FindAllMoves();
}

std::map<const coordinates_t, GraphItem> GameTable::GetMoveGraph() const
{
    return move_graph_;
}

void GameTable::PrintMoves(std::ostream &os)
{
    for (auto move_list : moves_)
    {
        for (auto move : move_list)
        {
            if (!move.IsStartMove())
            {
                os << move.GetMove() << " ";
            }
        }
        os << "\n";
    }
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
    coordinates_t next_cell = current_cell;
    Ball::CollisionResult collision = Ball::CollisionResult::Pass;
    Ball ball (INVALID_ID);

    do {
        current_cell = next_cell;
        collision = ball.CollisionWith(board_.at(current_cell), to);

        // If ball stands on the cell with hole during start of the move
        // it cannot fall to the hole now. It has fallen here
        // in previous move
        switch (collision)
        {
        case Ball::CollisionResult::FallToHoleOrPass:
            if (current_cell == start_from)
            {
                collision = Ball::CollisionResult::Pass;
            }
            break;
        case Ball::CollisionResult::FallToHoleOrStop:
            if (current_cell == start_from)
            {
                collision = Ball::CollisionResult::Stop;
            }

            break;
        case Ball::CollisionResult::Stop:
            // fallthrough
        case Ball::CollisionResult::Pass:
            // nothing to do here
            break;
        }

        next_cell = GetNeighbourCell(current_cell, to);
    } while (collision == Ball::CollisionResult::Pass);

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
        case Ball::CollisionResult::FallToHoleOrStop:
            collision = Ball::CollisionResult::Stop;
            gi.AddNeighbour(move_to, collision_cell);
            gi.AddHole(move_to, collision_cell);
            break;
        case Ball::CollisionResult::FallToHoleOrPass:
            gi.AddHole(move_to, collision_cell);
            collision_cell = GetNeighbourCell (collision_cell, move_to);
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

    os << "\nMove garph status:\n";

    auto graph = gt.GetMoveGraph();
    for (auto i : graph)
    {
        os << i.first << ": " << i.second << "\n";
        if (!i.second.GetHolesOnWayTo(Direction::North).empty())
        {
            os << "\t" << "Holes N: " << i.second.GetHolesOnWayTo(Direction::North)
               << "\n";
        }
        if (!i.second.GetHolesOnWayTo(Direction::West).empty())
        {
            os << "\t" << "Holes W: " << i.second.GetHolesOnWayTo(Direction::West)
               << "\n";
        }
        if (!i.second.GetHolesOnWayTo(Direction::South).empty())
        {
            os << "\t" << "Holes S: " << i.second.GetHolesOnWayTo(Direction::South)
               << "\n";
        }
        if (!i.second.GetHolesOnWayTo(Direction::East).empty())
        {
            os << "\t" << "Holes E: " << i.second.GetHolesOnWayTo(Direction::East)
               << "\n";
        }
    }

    os << "\n";

    return os;
}

void GameTable::FindAllMoves()
{
    //create a start item and start playing around
    std::map <coordinates_t, ball_id_t> balls;
    std::map <coordinates_t, ball_id_t> holes;

    for (auto ball : balls_)
    {
        balls.insert(std::make_pair(ball.first, ball.second.GetId()));
    }
    for (auto hole : holes_)
    {
        holes.insert(std::make_pair(hole.second, hole.first));
    }

    Movement start_point (balls, holes);
    std::list <Movement> moves = {start_point};
    SimulateGame(moves);
}


void GameTable::SimulateGame (std::list <Movement> & moves)
{
    if (IsTooLotMoves(moves))
    {
        // too lot of moves, better plays are saved
        return;
    }

    auto current_position = moves.back().GetBallsPositions();
    if (current_position.size() == 0)
    {
        //all balls are in the holes!
        SaveMoves(moves);
        return;
    }

    std::list <Movement> to_north = MakeMove(moves, Direction::North);
    std::list <Movement> to_west  = MakeMove(moves, Direction::West);
    std::list <Movement> to_south = MakeMove(moves, Direction::South);
    std::list <Movement> to_east  = MakeMove(moves, Direction::East);

    if (!to_north.empty())
    {
        SimulateGame(to_north);
    }
    if (!to_west.empty())
    {
        SimulateGame(to_west);
    }
    if (!to_south.empty())
    {
        SimulateGame(to_south);
    }
    if (!to_east.empty())
    {
        SimulateGame(to_east);
    }
}

bool GameTable::SaveMoves (const std::list <Movement> & moves)
{
    if ((moves_.size() == 0) ||
        (moves_.back().size() == moves.size() ))
    {
        moves_.push_back(moves);
        return true;
    }
    else if (moves_.back().size() > moves.size())
    {
        moves_.clear();
        moves_.push_back(moves);
        return true;
    }
    //cannot add: better moves are saved
    return false;
}

bool GameTable::IsTooLotMoves (const std::list <Movement> & moves)
{
    if ((moves_.size() == 0) ||
        (moves_.back().size() >= moves.size() ))
    {
        return false;
    }

    return true;
}

std::list <Movement>
GameTable::MakeMove (std::list <Movement> & moves, Direction to)
{
    auto current_position = moves.back().GetBallsPositions();
    if (current_position.size() == 0)
    {
        //all balls are in the holes! no need to proceed making moves
        SaveMoves(moves);
        return std::list <Movement> ();
    }

    std::map <coordinates_t, ball_id_t> new_position;
    std::map <coordinates_t, ball_id_t> new_position_removed_balls;
    bool game_ok = RollAllBalls (to,
                                 current_position,
                                 moves.back().GetHoles(),
                                 new_position,
                                 new_position_removed_balls);
    if (!game_ok)
    {
        return std::list <Movement> ();
    }

    moves.back().InitLoopGuard();

    Movement new_move (to, moves.back());
    for (auto ball : new_position_removed_balls)
    {
        for (auto previous : current_position )
        {
            if (ball.second == previous.second)
            {
                new_move.SetBallPosition(ball.second, ball.first, previous.first);
                break;
            }
        }
    }
    for (auto ball : new_position)
    {
        for (auto previous : current_position )
        {
            if (ball.second == previous.second)
            {
                new_move.SetBallPosition(ball.second, ball.first, previous.first);
                break;
            }
        }
    }

    if (new_move.IsLooped())
    {
        return std::list <Movement> ();
    }

    std::list <Movement> new_moves_list (moves);
    new_moves_list.push_back(new_move);
    return new_moves_list;
}


bool GameTable::RollAllBalls (Direction to,
                              const std::map <coordinates_t, ball_id_t> & current_position,
                              std::map <coordinates_t, ball_id_t> open_holes,
                              std::map <coordinates_t, ball_id_t> & new_position,
                              std::map <coordinates_t, ball_id_t> & new_position_removed)
{
    coordinate_t begin;
    coordinate_t end;
    bool x_before_y;

    switch (to)
    {
    case Direction::North:
        begin = 1;
        end   = table_size_;
        x_before_y = false;
        break;
    case Direction::West:
        begin = 1;
        end   = table_size_;
        x_before_y = true;
        break;
    case Direction::South:
        begin = table_size_;
        end   = 1;
        x_before_y = false;
        break;
    case Direction::East:
        begin = table_size_;
        end   = 1;
        x_before_y = true;
        break;
    }

    for (coordinate_t i=begin; i<=end; (begin < end) ? ++i : --i)
    {
        for (coordinate_t j=begin; j<=end; (begin < end) ? ++j : --j)
        {
            coordinates_t current_cell = (x_before_y) ? coordinates_t (i, j)
                                                      : coordinates_t (j, i);

            ball_id_t ball;
            auto ball_iterator = current_position.find(current_cell);
            if (ball_iterator == current_position.end())
            {
                //no ball here
                continue;
            }
            else
            {
                ball = ball_iterator->second;
            }

            coordinates_t next_hop = move_graph_.at(current_cell).GetNeigbour(to);
            auto gaps_on_road = move_graph_.at(current_cell).GetHolesOnWayTo(to);
            bool reach_gap = false;

            // ball can fall into the hole while movig
            // if hole id and ball's one dont match game lost,
            // otherwize ball in its hole and we are on our way to win
            for (auto gap : gaps_on_road)
            {
                //is gap open?
                auto hole = open_holes.find(gap);
                if (hole != open_holes.end())
                {
                    if (hole->second == ball)
                    {
                        // A-ha ball in his hole!
                        next_hop = hole->first;
                        reach_gap = true;
                        // block hole for next balls
                        open_holes.erase(hole);
                        break;
                    }
                    else
                    {
                        // Game over
                        return false;
                    }
                }
            }
            //todo several balls in one cell!!!

            // is cell empty?
            if (!reach_gap)
            {
                // cell can be occupied
                coordinates_t destination=next_hop;
                while ( (new_position.find(destination) != new_position.end()) &&
                        (destination != GetNeighbourCell(current_cell, ReverseDirection(to))) )
                {
                    destination = GetNeighbourCell(destination, ReverseDirection(to));
                }
                next_hop = destination;
            }

            // set new ball position
            if (reach_gap)
            {
                new_position_removed.insert(std::make_pair(next_hop, ball));
            }
            else
            {
                new_position.insert(std::make_pair(next_hop, ball));

            }
        }
    }
    return true;
}
