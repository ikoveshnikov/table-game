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
    FindBestMoves();
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
            os << move.GetMove() << " ";
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

bool GameTable::FindBestMoves()
{
    bool first = true;

    for (auto ball : balls_)
    {
        if (first)
        {
            moves_ = FindAllMoves(ball.second);
        }
        else
        {
            // TODO: we can parallel this cicle using intel TBB or OpenMP
            std::list <std::list <Movement> > checked_moves;
            for (auto moves : moves_)
            {
                auto checked = CheckMovesForBall(moves, ball.second);
                for (auto i : checked)
                {
                    checked_moves.push_back(i);
                }
            }
            moves_ = checked_moves;
        }
    }

    SaveBestMoves();

    return (!moves_.empty());
}

void GameTable::SaveBestMoves()
{
    std::list <std::list <Movement> > best_moves;
    size_t min_move_count = 0;

    for (auto i=moves_.begin(); i!=moves_.end(); ++i)
    {
        bool valid = false;
        for (auto move : *i)
        {
            valid = move.IsGood();
            if (!valid)
            {
                break;
            }
        }

        if (!valid)
        {
            continue;
        }

        // horaay! this move is valid
        if (best_moves.empty())
        {
            best_moves.push_back(*i);
            min_move_count = i->size();
        }
        else if (i->size() == min_move_count)
        {
            best_moves.push_back(*i);
        }
        else if (i->size() < min_move_count)
        {
            best_moves.clear();
            best_moves.push_back(*i);
            min_move_count = i->size();
        }
    }

    moves_ = best_moves;
}

std::list <std::list <Movement> >
GameTable::FindAllMoves (Ball & ball)
{
    //init start move

    std::map <coordinates_t, ball_id_t>  balls;
    std::map <coordinates_t, ball_id_t> holes;
    std::map <coordinates_t, bool>  visited_table;
    for (auto i : balls_)
    {
        balls.insert(std::make_pair(i.first, i.second.GetId()));
        if (i.second.GetId() == ball.GetId())
        {
            visited_table[i.first] = true;
        }
    }
    for (auto i : holes_)
    {
        holes[i.second] = i.first;
    }
    Movement start_move (balls, holes, visited_table);

    std::list <Movement> first_move = {start_move};
    std::list <std::list <Movement> > movement_list = {first_move};

    bool found_moves = false;
    do {
        found_moves = false;
        std::list <std::list <Movement> > temporal_list = movement_list;
        movement_list.clear();
        for (auto i : temporal_list)
        {
            std::list <std::list <Movement> >  new_moves = AddMoves(i, ball);
            for (auto j : new_moves)
            {
                movement_list.push_back(i);
            }
            if (new_moves.size() > 1)
            {
                found_moves = true;
            }
        }
    } while (found_moves);

    return movement_list;
}

std::list <std::list <Movement> >
GameTable::AddMoves (std::list <Movement> & moves,
                     Ball & ball)
{
    assert((moves.size() != 0));
    Movement & last_move = moves.back();
    const coordinates_t current_cell = last_move.GetBallPosition(ball.GetId());
    if (current_cell == holes_[ball.GetId()])
    {
        std::list <std::list <Movement> > move_list = {moves};
        return move_list;
    }

    std::list <std::list <Movement> > move_list;
    std::list <Movement> north_moves, west_moves, south_moves, east_moves;

    AddNextMove(last_move, north_moves, current_cell, Direction::North, ball);
    AddNextMove(last_move, west_moves,  current_cell, Direction::West,  ball);
    AddNextMove(last_move, south_moves, current_cell, Direction::South, ball);
    AddNextMove(last_move, east_moves,  current_cell, Direction::East,  ball);

    for (auto i : north_moves)
    {
        std::list <Movement> current_moves = moves;
        current_moves.push_back(i);
        move_list.push_back(current_moves);
    }

    for (auto i : west_moves)
    {
        std::list <Movement> current_moves = moves;
        current_moves.push_back(i);
        move_list.push_back(current_moves);
    }

    for (auto i : south_moves)
    {
        std::list <Movement> current_moves = moves;
        current_moves.push_back(i);
        move_list.push_back(current_moves);
    }

    for (auto i : east_moves)
    {
        std::list <Movement> current_moves = moves;
        current_moves.push_back(i);
        move_list.push_back(current_moves);
    }


    return move_list;
}

bool GameTable::AddNextMove (Movement & last_move, std::list <Movement > & next_move,
                             const coordinates_t & current_cell,
                             Direction to,
                             Ball & ball)
{
    if (current_cell == holes_[ball.GetId()])
    {
        //nothing to do. reached the target
        return true;
    }

    coordinates_t next_hop = move_graph_.at(current_cell).GetNeigbour(to);
    if ( next_hop == current_cell)
    {
        last_move.AddLoop(to);
        // no moves possible
        return false;
    }

    // check if we can reach destination in one step
    coordinates_t target_hole = holes_[ball.GetId()];
    coordinates_t max_hop = next_hop;
    std::vector <coordinates_t> reacheble_holes
            = move_graph_.at(current_cell).GetHolesOnWayTo(to);
    for (auto i :reacheble_holes)
    {
        if (i == target_hole)
        {
            max_hop = target_hole;
        }
    }

    // next hop is farest step we can make, so need to add several hopps
    // regarding some conditions

    for (auto nearest_hop = GetNeighbourCell(current_cell, to);
         nearest_hop <= max_hop;
         nearest_hop = GetNeighbourCell(nearest_hop, to))
    {
        Movement new_move (to, last_move);
        if (!new_move.SetBallPosition(ball.GetId(), nearest_hop, current_cell))
        {
            //movement can not to be done
            continue;
        }

        // ball will not stop by it's own
        if (nearest_hop != max_hop)
        {
            new_move.ReqireBallAt(GetNeighbourCell(nearest_hop, to));
        }
        // all holes need to be closed
        for (auto i : reacheble_holes)
        {
            if (i <= nearest_hop)
            {
                new_move.ReqireClosedHole(i);
            }
        }

        // now we have possible move derection with its own conditions
        next_move.push_back(new_move);


        if (nearest_hop == holes_[ball.GetId()])
        {
            //no need to go further
            break;
        }
    }
    return (!next_move.empty());
}

std::list <std::list <Movement> >
GameTable::CheckMovesForBall (std::list <Movement> & moves, Ball & ball)
{
    ball.GetId();
    std::list <std::list <Movement> > result = {moves};
    return result;
}

