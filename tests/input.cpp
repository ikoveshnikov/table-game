 #define BOOST_TEST_DYN_LINK
 #define BOOST_TEST_MODULE "TG_input"

#include <boost/test/unit_test.hpp>

#include "input.h"
#include "tg_utils.h"
#include "tests_config.h"



BOOST_AUTO_TEST_CASE( good_data )
{
    InputData data (sample);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::Ok);
    BOOST_CHECK_EQUAL(data.GetTableSize(), SAMPLE_TABLE_SIZE);
    BOOST_CHECK_EQUAL(data.GetBallCount(), SAMPLE_BALLS_COUNT);
    BOOST_CHECK_EQUAL(data.GetWallCount(), SAMPLE_WALLS_COUNT);

    std::vector <coordinates_t>  balls_sample;
    balls_sample.push_back({SAMPLE_BALL_1});
    balls_sample.push_back({SAMPLE_BALL_2});

    std::vector <coordinates_t> balls = data.GetBalls();
    BOOST_CHECK_EQUAL_COLLECTIONS(balls.begin(), balls.end(),
                                  balls_sample.begin(), balls_sample.end());

    std::vector <coordinates_t>  holes_sample;
    holes_sample.push_back({SAMPLE_HOLE_1});
    holes_sample.push_back({SAMPLE_HOLE_2});

    std::vector <coordinates_t>  holes = data.GetHoles();
    BOOST_CHECK_EQUAL_COLLECTIONS(holes.begin(), holes.end(),
                                  holes_sample.begin(), holes_sample.end());

    std::vector <wall_coordinates_t> walls_sample;
    walls_sample.push_back(wall_coordinates_t(SAMPLE_WALL_1));
    walls_sample.push_back(wall_coordinates_t(SAMPLE_WALL_2));

    std::vector<wall_coordinates_t> walls = data.GetWalls();
    BOOST_CHECK_EQUAL_COLLECTIONS(walls.begin(), walls.end(),
                                  walls_sample.begin(), walls_sample.end());

}

BOOST_AUTO_TEST_CASE (incomplite_1)
{
    InputData data (sample_incomplite_1);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::IncompliteData);
}

BOOST_AUTO_TEST_CASE (incomplite_2)
{
    InputData data (sample_incomplite_2);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::IncompliteData);
}

BOOST_AUTO_TEST_CASE (too_long)
{
    InputData data (sample_too_long);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::TooLongData);
}

BOOST_AUTO_TEST_CASE (invalid_1)
{
    InputData data (sample_invalid_1);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::InvalidCoordinates);
}

BOOST_AUTO_TEST_CASE (invalid_2)
{
    InputData data (sample_invalid_1);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::InvalidCoordinates);
}

BOOST_AUTO_TEST_CASE (double_1)
{
    InputData data (sample_double_1);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::Duplicates);
}

BOOST_AUTO_TEST_CASE (double_2)
{
    InputData data (sample_double_2);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::Duplicates);
}

BOOST_AUTO_TEST_CASE (double_3)
{
    InputData data (sample_double_3);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::Duplicates);
}

BOOST_AUTO_TEST_CASE (in_holes)
{
    InputData data (sample_in_holes);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::BallsInHoles);
}

BOOST_AUTO_TEST_CASE (no_balls)
{
    InputData data (sample_no_balls);

    BOOST_CHECK_EQUAL(data.GetDataStatus(), InputData::Status::NoBalls);
}
