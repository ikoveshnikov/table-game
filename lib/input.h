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

#ifndef TG_INPUT_H
#define TG_INPUT_H

#include <map>
#include <vector>
#include <string>

#include "tg_types.h"

class InputData
{
public:
    InputData (const input_t &input);
    ~InputData() = default;

    bool IsValid () const;
    std::string & GetErrorString();

    std::map<ball_id_t, coordinates_t> GetBalls() const;

    std::map<ball_id_t, coordinates_t> GetHoles() const;

    coordinate_t GetTableSize() const;

    ball_id_t GetBallCount() const;
    coordinate_t GetWallCount() const;


    std::vector<wall_coordinates_t> GetWalls() const;

private:
    std::map <ball_id_t, coordinates_t> balls_;
    std::map <ball_id_t, coordinates_t> holes_;
    coordinate_t table_size_;
    ball_id_t ball_count_;
    std::vector <wall_coordinates_t> walls_;
    coordinate_t walls_count_;

    std::string error_;
    void AppendError (const std::string & string);

    bool valid_;
    bool Validate ();

};

#endif // TG_INPUT_H
