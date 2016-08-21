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

class Movement
{
public:
    Movement(Direction to, const Movement & previous_move);
    Movement(const std::map <coordinates_t, ball_id_t> & balls,
             const std::map <coordinates_t, ball_id_t> & holes,
             std::map <coordinates_t, bool>  visited_table);
    ~Movement() = default;

    void AddLoop (Direction to);

    bool HasLoops (Direction side) const;

    void ReqireBallAt (const coordinates_t & cell);
    void ReqireClosedHole (const coordinates_t & cell);

    // is it good move? All reqirements must be met
    bool IsGood () const;
    // if move is start one almost all values will be invalid
    bool IsStartMove () const;

    // if ball fall into the hole, ball and hole will be erased from
    // corresponding cells.
    bool SetBallPosition(ball_id_t ball,
                         const coordinates_t &current_cell,
                         const coordinates_t &previous_cell);
    const coordinates_t GetBallPosition (ball_id_t ball);

    Direction GetMove () const;

    const std::map <coordinates_t, ball_id_t> & GetBallsPositions () const;
    const std::map <coordinates_t, ball_id_t> & GetHoles () const;

    void ClearVisitedTable ();
    std::map <coordinates_t, bool>  GetVisitedTable () const;

private:
    Direction move_;
    bool start_move_;

    // one-step loops. exist only if moovement to that direction wont
    // change positions of all the known balls
    bool loop_while_move_north_;
    bool loop_while_move_west_;
    bool loop_while_move_south_;
    bool loop_while_move_east_;

    // Movement posibility conditions:
    std::vector <coordinates_t> ball_on_way_;
    std::vector <coordinates_t> holes_closed_;

    //track where balls are
    std::map <coordinates_t, ball_id_t> occupied_cells_;
    //track which holes are still open
    std::map <coordinates_t, ball_id_t> holes_state_;

    // can be used only for one ball a time
    std::map <coordinates_t, bool> visited_;

    void RemoveReqiredBall (const coordinates_t & at);
};

#endif // TG_PATH_H
