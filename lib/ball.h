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

#ifndef TG_BALL_H
#define TG_BALL_H

#include "cell_object.h"
#include "board_cell.h"

//!
//! \brief The Ball class describes how ball can move on game board and
//! how it interacts with holes and wall on the board
//!
class Ball : public CellObject
{
public:
    //!
    //! \brief Ball Create ball object
    //! \param id ball's id. Valid values is 1->...
    //! if %id is set to 0, thet dummy object will be created
    //!
    Ball(ball_id_t id);
    ~Ball();

    //!
    //! \brief The CollisionResult enum Collision result with othres objects
    //! on game board
    //!
    enum class CollisionResult
    {
        Pass,               //!< No collision with this cell
        FallToHoleOrPass,   //!< Fall to the hole or pass if hole will be closed
        FallToHoleOrStop,   //!< Fall to the hole or stop in front of wall
        Stop                //!< Stop in front of the wall
    };

    //!
    //! \brief CollisionWith Gives result of collision on some cell, depending
    //! to the objects it has
    //! \param cell of game table
    //! \param move_to moving direction
    //! \return result of collision
    //!
    CollisionResult CollisionWith (const BoardCell & cell, Direction move_to) const;

    //!
    //! \brief GetId gives ball id attached to ball
    //! \return ball id
    //!
    ball_id_t GetId ();

private:
    //!
    //! \brief id_ attaced to the ball
    //!
    ball_id_t id_;
};

#endif //TG_BALL_H
