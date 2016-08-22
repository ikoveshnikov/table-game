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

//!
//! \brief The GameTable class Contains description of game state. Looking for
//! available moves sequence to win in this game
//!
class GameTable
{
public:
    //!
    //! \brief GameTable Create game table from input data.
    //! Input errors must be handled outside of this class
    //! \param in input data
    //!
    GameTable (const InputData & in);
    ~GameTable() = default;

    //!
    //! \brief GetBoard gives representation of game board: cells with their
    //! walls and holes
    //! \return game board representation
    //!
    std::map<const coordinates_t, BoardCell> GetBoard() const;

    //!
    //! \brief GetTableSize Gives size of game board table
    //! \return game board size
    //!
    coordinate_t GetTableSize() const;

    //!
    //! \brief GetBalls return initial state of balls on the cell
    //! \return initial state of the balls
    //!
    const std::map <coordinates_t, Ball> & GetBalls() const;

    //!
    //! \brief CalculateMoves calculate moves based on initial board and balls
    //! state. Must be called manually
    //!
    void CalculateMoves ();

    //!
    //! \brief GetMoveGraph gives representation of internal move graph
    //! \return return move graph
    //!
    std::map<const coordinates_t, GraphItem> GetMoveGraph() const;

    //!
    //! \brief PrintMoves prints moves sequence to win in this game
    //! \param os output stream
    //!
    void PrintMoves (std::ostream & os);

protected:
    //! \brief board_ initial board state
    std::map <const coordinates_t, BoardCell> board_;

    //! \brief balls_ initial position of balls
    std::map <coordinates_t, Ball> balls_;

    //! \brief move_graph_ get move move graph builded based on initial board
    //! state
    std::map <const coordinates_t, GraphItem> move_graph_;

    //! \brief table_size_ size of board table
    coordinate_t table_size_;

    //! \brief moves_ best moves sequences
    std::list <std::list <Movement> > moves_;

    //! \brief holes_ initial holes positions
    std::map <ball_id_t, coordinates_t> holes_;

    //!
    //! \brief BuildMoveGraph build movement graph using initial board state
    //!
    void BuildMoveGraph ();

    //!
    //! \brief RollBall Roll ball from current position in specified direction
    //! \param start_from start move position
    //! \param to desired direction
    //! \return cell where ball will stop and why
    //!
    std::pair <Ball::CollisionResult, coordinates_t>
    RollBall(const coordinates_t & start_from,
             const Direction to) const;

    //!
    //! \brief FillGraphItemInDirection Fiil graph node in specified direction
    //! \param gi graph node
    //! \param current_cell cell graph node attached to
    //! \param move_to direction
    //!
    void FillGraphItemInDirection (GraphItem & gi,
                                   coordinates_t current_cell,
                                   Direction move_to) const;

    //!
    //! \brief FindAllMoves find best sequince of moves to win the game
    //!
    void FindAllMoves ();

    //!
    //! \brief SimulateGame Simulate game untill best moves are found or no
    //! more possible moves. Makes BFS search in move graph simultaniously
    //! for several nodes
    //! \param moves
    //!
    void SimulateGame (std::list <std::list<Movement> > &moves);

    //!
    //! \brief SaveMoves save move sequence pretending to be one of the best
    //! \param moves moves sequence
    //! \return false if cannot add
    //!
    bool SaveMoves (const std::list <Movement> & moves);

    //!
    //! \brief IsTooLotMoves check if current moves sequence is longer than
    //! known best ones. If so no need to process that sequence longer
    //! \param moves moves sequence
    //! \return true if too long, false in not
    //!
    bool IsTooLotMoves (const std::list <Movement> & moves);

    //!
    //! \brief RollAllBalls Roll all balls to specific direction and get balls
    //! position after move
    //! \param to move direction
    //! \param current_position current position of the balls
    //! \param open_holes currently open holes
    //! \param new_position balls position after roll
    //! \param new_position_removed some of the balls fall in the holes, thats
    //! there they have anded their path
    //! \return false if game will be lost during that movement, true if
    //! roll gives valid game state
    //!
    bool RollAllBalls (Direction to,
                       const std::map <coordinates_t, ball_id_t> & current_position,
                       std::map<coordinates_t, ball_id_t> open_holes,
                       std::map<coordinates_t, ball_id_t> &new_position,
                       std::map<coordinates_t, ball_id_t> &new_position_removed);

    //!
    //! \brief MakeMove make one roll to the desired direction and form sequence
    //! with that move included
    //! \param moves current moves sequence
    //! \param to direction of new move
    //! \return sequence of moves with new move attached, empty list if move
    //! cannot be done
    //!
    std::list <Movement> MakeMove (std::list<Movement> &moves, Direction to);
};

std::ostream &
operator << (std::ostream & os, const GameTable & gt);

#endif // TG_TABLE_H
