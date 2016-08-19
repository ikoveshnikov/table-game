#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "TG_utils"

#include <boost/test/unit_test.hpp>

#include "tg_utils.h"



BOOST_AUTO_TEST_CASE( coordinates_equal )
{
    coordinates_t one   (5, 5);
    coordinates_t two   (5, 6);
    coordinates_t three (5, 5);

    BOOST_CHECK_EQUAL(one   == two,   false);
    BOOST_CHECK_EQUAL(one   == three, true);
    BOOST_CHECK_EQUAL(three == two,   false);
}

BOOST_AUTO_TEST_CASE( coordinates_not_equal )
{
    coordinates_t one   (5, 5);
    coordinates_t two   (5, 6);
    coordinates_t three (5, 5);

    BOOST_CHECK_EQUAL(one   != two,   true);
    BOOST_CHECK_EQUAL(one   != three, false);
    BOOST_CHECK_EQUAL(three != two,   true);
}

BOOST_AUTO_TEST_CASE( coordinates_less )
{
    coordinates_t one   (1, 1);
    coordinates_t two   (1, 2);
    coordinates_t three (2, 1);
    coordinates_t four  (2, 2);

    BOOST_CHECK_EQUAL(one < one,   false);
    BOOST_CHECK_EQUAL(one < two,   true);
    BOOST_CHECK_EQUAL(one < three, true);
    BOOST_CHECK_EQUAL(one < four,  true);

    BOOST_CHECK_EQUAL(two < one,   false);
    BOOST_CHECK_EQUAL(two < two,   false);
    BOOST_CHECK_EQUAL(two < three, true);
    BOOST_CHECK_EQUAL(two < four,  true);

    BOOST_CHECK_EQUAL(three < one,   false);
    BOOST_CHECK_EQUAL(three < two,   false);
    BOOST_CHECK_EQUAL(three < three, false);
    BOOST_CHECK_EQUAL(three < four,  true);

    BOOST_CHECK_EQUAL(four < one,   false);
    BOOST_CHECK_EQUAL(four < two,   false);
    BOOST_CHECK_EQUAL(four < three, false);
    BOOST_CHECK_EQUAL(four < four,  false);
}

BOOST_AUTO_TEST_CASE( walls_equal )
{
    wall_coordinates_t one    (3, 2, 4, 2);
    wall_coordinates_t two    (4, 2, 3, 2);
    wall_coordinates_t three  (3, 2, 3, 3);

    BOOST_CHECK_EQUAL(one   == two,   true);
    BOOST_CHECK_EQUAL(one   == three, false);
    BOOST_CHECK_EQUAL(three == two,   false);
}

BOOST_AUTO_TEST_CASE( walls_not_equal )
{
    wall_coordinates_t one    (3, 2, 4, 2);
    wall_coordinates_t two    (4, 2, 3, 2);
    wall_coordinates_t three  (3, 2, 3, 3);

    BOOST_CHECK_EQUAL(one   != two,   false);
    BOOST_CHECK_EQUAL(one   != three, true);
    BOOST_CHECK_EQUAL(three != two,   true);
}

BOOST_AUTO_TEST_CASE( coordinates_valid )
{
    coordinate_t table_size = 8;
    coordinates_t c_1  (0,            0);
    coordinates_t c_2  (0,            table_size/2);
    coordinates_t c_3  (table_size/2, 0);
    coordinates_t c_4  (table_size/2, table_size/2);
    coordinates_t c_5  (table_size,   table_size);
    coordinates_t c_6  (table_size,   table_size+1);
    coordinates_t c_7  (table_size+1, table_size);
    coordinates_t c_8  (table_size+1, table_size+1);

    BOOST_CHECK_EQUAL(IsValid(c_1, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(c_2, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(c_3, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(c_4, table_size), true);
    BOOST_CHECK_EQUAL(IsValid(c_5, table_size), true);
    BOOST_CHECK_EQUAL(IsValid(c_6, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(c_7, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(c_8, table_size), false);
}

BOOST_AUTO_TEST_CASE( cells_neigbours )
{
    coordinates_t cell (5, 5);
    coordinates_t n_1  (5, 4);
    coordinates_t n_2  (5, 6);
    coordinates_t n_3  (4, 5);
    coordinates_t n_4  (6, 5);

    coordinates_t nn_1  (5, 3);
    coordinates_t nn_2  (2, 6);
    coordinates_t nn_3  (8, 5);
    coordinates_t nn_4  (6, 6);

    BOOST_CHECK_EQUAL(IsNeigbours(cell, cell), false);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, n_1),  true);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, n_2),  true);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, n_3),  true);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, n_4),  true);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, nn_1), false);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, nn_2), false);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, nn_3), false);
    BOOST_CHECK_EQUAL(IsNeigbours(cell, nn_4), false);
}

BOOST_AUTO_TEST_CASE( walls_valid )
{
    coordinate_t table_size = 8;

    coordinates_t cell (5, 5);
    coordinates_t n_1  (5, 4);
    coordinates_t n_2  (5, 6);
    coordinates_t n_3  (4, 5);
    coordinates_t n_4  (6, 5);

    coordinates_t nn_1  (5, 3);
    coordinates_t nn_2  (2, 6);
    coordinates_t nn_3  (8, 5);
    coordinates_t nn_4  (6, 6);

    //valid walls
    wall_coordinates_t w_1 (cell, n_1);
    wall_coordinates_t w_2 (cell, n_2);
    wall_coordinates_t w_3 (cell, n_3);
    wall_coordinates_t w_4 (cell, n_4);

    //invalid walls
    wall_coordinates_t iw_1 (cell, nn_1);
    wall_coordinates_t iw_2 (cell, nn_2);
    wall_coordinates_t iw_3 (cell, nn_3);
    wall_coordinates_t iw_4 (cell, nn_4);
    wall_coordinates_t iw_5 (cell, cell);

    BOOST_CHECK_EQUAL(IsValid(w_1,  table_size), true);
    BOOST_CHECK_EQUAL(IsValid(w_1,  table_size), true);
    BOOST_CHECK_EQUAL(IsValid(w_3,  table_size), true);
    BOOST_CHECK_EQUAL(IsValid(w_4,  table_size), true);
    BOOST_CHECK_EQUAL(IsValid(iw_1, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(iw_2, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(iw_3, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(iw_4, table_size), false);
    BOOST_CHECK_EQUAL(IsValid(iw_5, table_size), false);
}

BOOST_AUTO_TEST_CASE( duplicates )
{
    std::vector <int> v_no_dup = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    BOOST_CHECK_EQUAL(HasDuplicates(v_no_dup), false);

    std::vector <int> v_dup = {1, 1, 3, 4, 5, 6, 7, 8, 9};

    BOOST_CHECK_EQUAL(HasDuplicates(v_dup), true);
}

