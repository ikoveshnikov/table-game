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

#ifndef TG_PATH_H
#define TG_PATH_H

#include <vector>
#include <map>

#include "tg_types.h"

//!
//! \brief The Movement class stores state during computing possible movements
//!
class Movement
{
public:
    //!
    //! \brief Movement creates movement from previous state
    //! \param to move direction
    //! \param previous_move previous board state
    //!
    Movement(Direction to, const Movement & previous_move);

    //!
    //! \brief Movement creates state as initial position to prepare for calculations
    //! \param balls coordinates of balls and their's ids
    //! \param holes coordinates of holes and their's ids
    //!
    Movement(const std::map <coordinates_t, ball_id_t> & balls,
             const std::map <coordinates_t, ball_id_t> & holes);
    ~Movement() = default;

    //!
    //! \brief IsStartMove Start items describes only initial state of board
    //! and can not tell where to go. Move direction will have invalid value
    //! \return true if item is initial state
    //!
    bool IsStartMove () const;

    //!
    //! \brief SetBallPosition update ball position on the board.
    //! if ball will stend up on cell wth hole, they will destroy each other.
    //! Caller must be worried abot holes between start and stop positions
    //! \param ball ball id
    //! \param current_cell current position on the cell
    //! \param previous_cell previous position on the cell
    //! \return false if move is invalid
    //!
    bool SetBallPosition(ball_id_t ball,
                         const coordinates_t &current_cell,
                         const coordinates_t &previous_cell);

    //!
    //! \brief GetBallPosition Get current position for specific ball
    //! \param ball id of the ball
    //! \return cell address occupied by the ball. return (0,0) if
    //! ball not found
    //!
    const coordinates_t GetBallPosition (ball_id_t ball);

    //!
    //! \brief GetMove Get move direction. Has invalid value for start moves
    //! \return movement direction
    //!
    Direction GetMove () const;

    //!
    //! \brief GetBallsPositions Return current position of all balls. If ball
    //! reached it's hole it will not be present in in return value
    //! \return current position of all balls
    //!
    const std::map <coordinates_t, ball_id_t> & GetBallsPositions () const;

    //!
    //! \brief GetHoles Return current state of all states of all the holes.
    //! If hole is closed by the ball it will not be present in return value;
    //! \return current state of all holes
    //!
    const std::map <coordinates_t, ball_id_t> & GetHoles () const;

    //!
    //! \brief The LoopGuard struct Keep track of loops for all the mooves
    //!
    struct LoopGuard
    {
        //! \brief visited_cells_count_ tracks difference from current
        //! count of visited cells
        size_t visited_cells_count_;

        //! \brief visited_cells_ visited cells. each item has value 'true'
        //! if cell was visited, otherwise corresponding value is not set
        std::map <coordinates_t, bool> visited_cells_;
    };

    //!
    //! \brief GetLoopGuard return current state of loop guard
    //! \return state of loop guard
    //!
    const std::map <ball_id_t, LoopGuard>  GetLoopGuard () const;

    //!
    //! \brief InitLoopGuard need to be called before using loop guard.
    //! Call init of loop guard, update current positions of balls and then
    //! check for loops with %IsLooped()
    //!
    void InitLoopGuard ();

    //!
    //! \brief IsLooped Check if last moves caused game state to be looped.
    //! First init loop guard by calling  %InitLoopGuard(), update current
    //! ball positions with %SetBallPosition(). After that safe to use %IsLooped()
    //! check
    //!
    //! If updating ball positions doesnot make balls to visit new cell,
    //! state just got stack in loop and game will never reach win position
    //! \return bool true if state was looped
    //!
    bool IsLooped () const;


private:
    //!
    //! \brief move_ Describes last move to to get current state. Invalid value
    //! if movement is start position
    //!
    Direction move_;

    //!
    //! \brief start_move_ If true item describes only initial state.
    //! Field direction is invalid
    //!
    bool start_move_;

    //!
    //! \brief occupied_cells_ Describes current ball positions on this move
    //!
    std::map <coordinates_t, ball_id_t> occupied_cells_;

    //!
    //! \brief holes_state_ Describes open holes on this move
    //!
    std::map <coordinates_t, ball_id_t> holes_state_;

    //!
    //! \brief loop_guard_ per-ball loop guard
    //!
    std::map <ball_id_t, LoopGuard> loop_guard_;

};

#endif // TG_PATH_H
