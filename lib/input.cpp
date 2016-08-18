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
#include "tg_utils.h"

InputData::InputData(const input_data_t &input)
    : status_(Status::Ok)
{
    size_t minimum_size = 3;
    if (input.size() < minimum_size)
    {
        status_ = Status::IncompliteData;
        return;
    }

    table_size_  = input.at(0);
    ball_count_  = input.at(1);
    walls_count_ = input.at(2);

    // 4 coordinates for every  ball: ball's coordinate, hole's coordinates
    // 4 coordinates for every wall
    size_t expected_data_size =  minimum_size + ball_count_ * 4 + walls_count_ * 4;
    if (input.size() < expected_data_size)
    {
        status_ = Status::IncompliteData;
        return;
    }
    else if (input.size() > expected_data_size)
    {
        status_ = Status::TooLongData;
        return;
    }
    else if (ball_count_ == 0)
    {
        status_ = Status::NoBalls;
        return;
    }

    balls_.reserve(ball_count_);
    holes_.reserve(ball_count_);
    walls_.reserve(walls_count_);

    for (ball_id_t i=0; i<ball_count_; ++i)
    {
        size_t current_position = minimum_size + i * 2;
        coordinate_t x (input.at(current_position));
        coordinate_t y (input.at(current_position +1));
        coordinates_t coords (x, y);
        if (! IsValid(coords, table_size_))
        {
            status_ = Status::InvalidCoordinates;
            return;
        }
        balls_.push_back(coords);
    }

    for (ball_id_t i=0; i<ball_count_; ++i)
    {
        size_t current_position = minimum_size + ball_count_ *2 + i * 2;
        coordinate_t x (input.at(current_position));
        coordinate_t y (input.at(current_position +1));
        coordinates_t coords (x, y);
        if (! IsValid(coords, table_size_))
        {
            status_ = Status::InvalidCoordinates;
            return;
        }
        holes_.push_back(coords);
    }

    size_t start_of_walls = minimum_size + ball_count_ * 4;

    if (((input.size() - start_of_walls) % 4 != 0) ||
        walls_count_ != (input.size() - start_of_walls) / 4)
    {
        status_ = Status::IncompliteData;
        return;
    }

    for (coordinate_t i=0; i<walls_count_; ++i)
    {
        size_t current_position = minimum_size  + ball_count_ * 4 + i*4;
        coordinate_t x1 (input.at(current_position));
        coordinate_t y1 (input.at(current_position +1));
        coordinate_t x2 (input.at(current_position +2));
        coordinate_t y2 (input.at(current_position +3));
        wall_coordinates_t wall (x1, y1, x2, y2);
        if (! IsValid(wall, table_size_))
        {
            status_ = Status::InvalidCoordinates;
            return;
        }
        walls_.push_back(wall);
    }

    Validate();
}

InputData::Status InputData::GetDataStatus() const
{
    return status_;
}

const std::string InputData::GetErrorString() const
{
    switch (status_)
    {
    case Status::Ok:
        return "Input data is OK";
    case Status::IncompliteData:
        return "Input data is too short. Check if all the coordinates present.";
    case Status::TooLongData:
        return "Input data is too long. Check if there is no extra coordinates.";
    case Status::BallsInHoles:
        return "Some balls are already in the holes.";
    case Status::NoBalls:
        return "There is no balls in input data set";
    case Status::Duplicates:
        return "Some objects has duplicates!";
    case Status::InvalidCoordinates:
        return  "Some coordinates are less than 1 or bigger than table size.";
    }

    return "";
}

std::vector<coordinates_t> InputData::GetBalls() const
{
    return balls_;
}

std::vector<coordinates_t> InputData::GetHoles() const
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

void InputData::Validate()
{
    if (FindDuplicates(balls_))
    {
        status_ = Status::Duplicates;
        return;
    }
    if (FindDuplicates(holes_))
    {
        status_ = Status::Duplicates;
        return;
    }

    if (FindDuplicates(walls_))
    {
        status_ = Status::Duplicates;
        return;
    }

    // Board state still can be invalid, if one of the balls will
    // stay up on the hole
    for (auto i : balls_)
    {
        for (auto j : holes_)
        {
            if (i == j)
            {
                status_ = Status::BallsInHoles;
                return;
            }
        }
    }
}

std::ostream &
operator<<(std::ostream & os, const InputData::Status & s)
{
    switch (s)
    {
    case InputData::Status::Ok:
        os << "Status: Ok";
        break;
    case InputData::Status::IncompliteData:
        os << "Status: IncompliteData";
        break;
    case InputData::Status::TooLongData:
        os << "Status: TooLongData";
        break;
    case InputData::Status::InvalidCoordinates:
        os << "Status: InvalidCoordinates";
        break;
    case InputData::Status::Duplicates:
        os << "Status: Duplicates";
        break;
    case InputData::Status::BallsInHoles:
        os << "Status: BallsInHoles";
        break;
    case InputData::Status::NoBalls:
        os << "Status: NoBalls";
        break;
    }
    return os;
}
