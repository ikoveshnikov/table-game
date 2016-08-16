 #define BOOST_TEST_DYN_LINK
 #define BOOST_TEST_MODULE "OrionCore_CmdLine"

#include <boost/test/unit_test.hpp>

#include "input.h"


BOOST_AUTO_TEST_CASE( input_data )
{
    input_t in = {4, 2, 2, 2, 2, 1, 4, 1, 1, 4, 3, 1, 2, 1, 3, 3, 2, 4, 2};

    InputData data (in);

    BOOST_CHECK_EQUAL(data.IsValid(), true);
    BOOST_CHECK_EQUAL(data.GetTableSize(), 4);
    BOOST_CHECK_EQUAL(data.GetBallCount(), 2);
    BOOST_CHECK_EQUAL(data.GetWallCount(), 2);

    coordinates_t ball_1 (2,2);
    coordinates_t ball_2 (1,4);

    coordinates_t hole_1 (1,1);
    coordinates_t hole_2 (4,3);

    BOOST_CHECK(data.GetBalls().at(0) == ball_1);
    BOOST_CHECK(data.GetBalls().at(1) == ball_2);
    BOOST_CHECK(data.GetHoles().at(0) == hole_1);
    BOOST_CHECK(data.GetHoles().at(1) == hole_2);

    wall_coordinates_t wall_1 {{1,2}, {1,3}};
    wall_coordinates_t wall_2 {{3,2}, {4,2}};

    BOOST_CHECK(data.GetWalls().at(0) == wall_1);
    BOOST_CHECK(data.GetWalls().at(1) == wall_2);

}
