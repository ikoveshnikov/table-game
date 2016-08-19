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
#include <ostream>

#include "tg_types.h"

//!
//! \brief The InputData class reads input vector of coordinates
//! and makes base checks of boundaries and data consistence
//!
class InputData
{
public:
    //!
    //! \brief InputData creates input data object and makes data validation
    //! \param input vector of coordinates
    //!
    InputData (const input_data_t &input);
    ~InputData() = default;

    //!
    //! \brief The Status enum Describes status of parsing input data
    //!
    enum class Status
    {
        Ok,                 //!< No errors in input data
        IncompliteData,     //!< Coordinates pattern is too short
        TooLongData,        //!< Coordinates pattern is too long
        InvalidCoordinates, //!< Some of coordinates in input data are invalid
        Duplicates,         //!< Some of objecta are duplicated
        BallsInHoles,       //!< Some balls are already in holes
        NoBalls             //!< No balls on the board. Nothing to play with
    };

    //!
    //! \brief GetDataStatus Returns parsing status of input data
    //! \return parsing status description. See %Status for more information
    //!
    Status GetDataStatus () const;

    //!
    //! \brief GetErrorString Human-readable representation of parsing status
    //! \return string with error description
    //!
    const std::string GetErrorString() const;

    //!
    //! \brief GetTableSize Describes size of the table
    //! \return size of the game board
    //!
    coordinate_t GetTableSize() const;

    //!
    //! \brief GetBallCount Returns amount of balls and holes on the table.
    //! Count of balls and holes in the same cannot be bigger than number
    //! of cells on the board, otherways at least one object will be duplicated.
    //! If happened %status_ will be set to corresponding value
    //! \return value from 1 to %table_size_ ^ 2 / 2
    //!
    ball_id_t GetBallCount() const;

    //!
    //! \brief GetWallCount Returns amount of walls on game board
    //! Same as %GetBallCount(): if overflow maximum possible cout puplicate
    //! error will be shown
    //! \return  value from 1 to ( %table_size_ -1 ) ^ 2 * 2
    //!
    coordinate_t GetWallCount() const;

    //!
    //! \brief GetBalls returns all available balls on the game board
    //! \return Array of board coordinates where balls ere installed.
    //! Vectoe index describes ball's id
    //!
    std::vector<coordinates_t> GetBalls() const;

    //!
    //! \brief GetHoles Same as %GetBalls
    //! \return Array of board coordinates where holes ere installed.
    //! Vectoe index describes halls's id
    //!
    std::vector<coordinates_t> GetHoles() const;

    //!
    //! \brief GetWalls Same as %GetBalls
    //! \return rray of board coordinates where bordedrs ere installed.
    //!
    std::vector<wall_coordinates_t> GetWalls() const;

private:
    //!
    //! \brief table_size_ Describes size of game board
    //!
    coordinate_t table_size_;

    //!
    //! \brief balls_ Balls coordinates on board
    //!
    std::vector <coordinates_t> balls_;

    //!
    //! \brief holes_ Holes coordinates on board
    //!
    std::vector <coordinates_t> holes_;

    //!
    //! \brief walls_ Walls installed on board. Doesnot contain border walls
    //!
    std::vector <wall_coordinates_t> walls_;

    //!
    //! \brief status_ Describes parsing status of input data
    //!
    Status status_;

    //!
    //! \brief Validate input data for consistence: having no dublicates,
    //! having only one object on cell
    //!
    void Validate();
};

std::ostream &
operator<<(std::ostream & os, const InputData::Status & s);

#endif // TG_INPUT_H
