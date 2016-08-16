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

#include "input.h"

InputData::InputData(const input_t &input)
    : valid_(false)
{
    size_t minimum_size = 3;
    if (input.size() < minimum_size)
    {
        AppendError("Too small data set: at least table size and ball count"
                    "must be provided!");
        return;
    }

    table_size_  = input.at(0);
    ball_count_  = input.at(1);
    walls_count_ = input.at(2);

    if (input.size() < minimum_size + ball_count_ * 4)
    {
        AppendError("Too small data set: not enough balls and holes description!");
        return;
    }

    for (ball_id_t i=0; i<ball_count_; ++i)
    {
        size_t current_position = minimum_size + i * 2;
        coordinate_t x (input.at(current_position));
        coordinate_t y (input.at(current_position +1));
        coordinates_t coords (x, y);
        balls_[i] = coords;
    }

    for (ball_id_t i=0; i<ball_count_; ++i)
    {
        size_t current_position = minimum_size + ball_count_ *2 + i * 2;
        coordinate_t x (input.at(current_position));
        coordinate_t y (input.at(current_position +1));
        coordinates_t coords (x, y);
        holes_[i] = coords;
    }

    size_t start_of_walls = minimum_size + ball_count_ * 4;

    if (((input.size() - start_of_walls) % 4 != 0) ||
        walls_count_ != (input.size() - start_of_walls) / 4)
    {
        AppendError("Too small data set: walls are broken!");
        return;
    }

    for (coordinate_t i=0; i<walls_count_; ++i)
    {
        size_t current_position = minimum_size  + ball_count_ * 4 + i*4;
        coordinate_t x1 (input.at(current_position));
        coordinate_t y1 (input.at(current_position +1));
        coordinate_t x2 (input.at(current_position +2));
        coordinate_t y2 (input.at(current_position +3));
        coordinates_t coords1 (x1, y1);
        coordinates_t coords2 (x2, y2);
        walls_.push_back(std::make_pair(coords1, coords2));
    }

    valid_ = Validate();
}

bool InputData::IsValid() const
{
    return valid_;
}

std::string &InputData::GetErrorString()
{
    return error_;
}

std::map<ball_id_t, coordinates_t> InputData::GetBalls() const
{
    return balls_;
}

std::map<ball_id_t, coordinates_t> InputData::GetHoles() const
{
    return holes_;
}

coordinate_t InputData::GetTableSize() const
{
    return table_size_;
}

ball_id_t InputData::GetBallCount() const
{
    return ball_count_;
}

coordinate_t InputData::GetWallCount() const
{
    return walls_count_;
}

std::vector<wall_coordinates_t> InputData::GetWalls() const
{
    return walls_;
}

void InputData::AppendError(const std::string &string)
{
    error_.append(string);
    error_.append("\n");
}

bool InputData::Validate()
{
    valid_ = true;
    // balls, holes and walls coordinates must belong to [1, table_size_]
    for (auto i : balls_)
    {
        const coordinates_t & c = i.second;
        if ((c.first  < 1) || (c.first  > table_size_) ||
            (c.second < 1) || (c.second > table_size_) )
        {
            valid_ = false;
            AppendError("Ball #" + std::to_string(i.first) + " has invalid coordinates");
        }
    }

    for (auto i : holes_)
    {
        const coordinates_t & c = i.second;
        if ((c.first  < 1) || (c.first  > table_size_) ||
            (c.second < 1) || (c.second > table_size_) )
        {
            valid_ = false;
            AppendError("Hole #" + std::to_string(i.first) + " has invalid coordinates");
        }
    }

    size_t j = 0;
    for (auto i : walls_)
    {
        const coordinates_t & c1 = i.first;
        const coordinates_t & c2 = i.second;

        if ((c1.first  < 1) || (c1.first  > table_size_) ||
            (c1.second < 1) || (c1.second > table_size_) ||
            (c2.first  < 1) || (c2.first  > table_size_) ||
            (c2.second < 1) || (c2.second > table_size_) )
        {
            valid_ = false;
            AppendError("Wall #" + std::to_string(j) + " has invalid coordinates");
        }
        ++j;
    }

    // TODO: board still can be invalid, if one of the balls will
    // stay up on inapropriate hole

    return valid_;
}
