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

#ifndef TG_TABLE_H
#define TG_TABLE_H

#include <map>
#include <vector>
#include <string>
#include <map>
#include <list>

#include "tg_types.h"
#include "cell_object.h"
#include "input.h"
#include "board_cell.h"
#include "ball.h"
#include "move_graph.h"
#include "movement.h"

class GameTable
{
public:
    GameTable (const InputData & in);
    ~GameTable() = default;


    std::map<const coordinates_t, BoardCell> GetBoard() const;

    coordinate_t GetTableSize() const;

    const std::map <coordinates_t, Ball> & GetBalls() const;

    void CalculateMoves ();

    std::map<const coordinates_t, GraphItem> GetMoveGraph() const;

    void PrintMoves (std::ostream & os);

protected:
    std::map <const coordinates_t, BoardCell> board_;
    std::map <coordinates_t, Ball> balls_;
    std::map <const coordinates_t, GraphItem> move_graph_;
    coordinate_t table_size_;

    std::list <std::list <Movement> > moves_;
    std::map <ball_id_t, coordinates_t> holes_;


    void BuildMoveGraph ();

    std::pair <Ball::CollisionResult, coordinates_t>
    RollBall(const coordinates_t & start_from,
             const Direction to) const;
    void FillGraphItemInDirection (GraphItem & gi, coordinates_t current_cell, Direction move_to) const;

    // all about movement
    void FindAllMoves ();
    void SimulateGame (std::list <std::list<Movement> > &moves);
    bool SaveMoves (const std::list <Movement> & moves);
    bool IsTooLotMoves (const std::list <Movement> & moves);
    bool RollAllBalls (Direction to,
                       const std::map <coordinates_t, ball_id_t> & current_position,
                       std::map<coordinates_t, ball_id_t> open_holes,
                       std::map<coordinates_t, ball_id_t> &new_position, std::map<coordinates_t, ball_id_t> &new_position_removed);
    std::list <Movement> MakeMove (std::list<Movement> &moves, Direction to);
};

std::ostream &
operator << (std::ostream & os, const GameTable & gt);

#endif // TG_TABLE_H
