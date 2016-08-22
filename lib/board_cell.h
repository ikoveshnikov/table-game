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

#ifndef TG_BOARD_CELL_H
#define TG_BOARD_CELL_H

#include "cell_object.h"

//!
//! \brief The BoardCell class describes cells on the game board
//!
class BoardCell : public CellObject
{
public:
    //!
    //! \brief The Walls struct describes walls configuration on the cell
    //!
    struct Walls
    {
        bool north; //!< Wall on north side
        bool west;  //!< Wall on the west side
        bool south; //!< wall on the south side
        bool east;  //!< Wall on the east side
    };

    //!
    //! \brief BoardCell create cell with cpecific wall configuration
    //! \param w wall configuration
    //!
    BoardCell (Walls w);

    //!
    //! \brief BoardCell create cell without walls
    //!
    BoardCell ();

    ~BoardCell ();

    //!
    //! \brief AddWall add wall on the cell on specific direction
    //! Neigbour cell must have wall on corresponding side
    //! \param at side where to place the wall
    //!
    void AddWall (Direction at);

    //!
    //! \brief HasWall Check if cell have walls on requested side
    //! \param at side
    //! \return true if wall present. false otherwise
    //!
    bool HasWall (Direction at) const;

    //!
    //! \brief AddHole Add hole for scpecific ball id
    //! Every cell can have only one ball
    //! \param id of the hole
    //!
    void AddHole (ball_id_t id);

    //!
    //! \brief HasHole Check if board cell has holes
    //! \return true if cell has hole
    //!
    bool HasHole () const;

    //!
    //! \brief HoleId returns hole id if it is attached to the cell
    //! \return Hole id, 0 if cell has no hole
    //!
    ball_id_t HoleId() const;

protected:
    //!
    //! \brief walls_ Wall configurations on the cell
    //!
    Walls walls_;

    //!
    //! \brief has_hole_ true if cell has hole
    //!
    bool has_hole_;

    //!
    //! \brief hole_id_ hole id if cell has hole, 0 otherwise
    //!
    ball_id_t hole_id_;
};

#endif //TG_BOARD_CELL_H
