#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "TG_table"

#include <boost/test/unit_test.hpp>

#include "table.h"
#include "tests_config.h"
#include "tg_utils.h"

class CheckTable : public GameTable
{
public:
    CheckTable() : GameTable(sample) {}
    ~CheckTable() = default;

    void RunCheck ()
    {
        CalculateMoves();
        CkeckMoveGraph ();
    }

    void CkeckMoveGraph()
    {
        // First check connections between cells
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetNeigbour(Direction::North),
                          coordinates_t(1,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetNeigbour(Direction::West),
                          coordinates_t(1,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetNeigbour(Direction::South),
                          coordinates_t(1,2));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetNeigbour(Direction::East),
                          coordinates_t(4,1));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetNeigbour(Direction::North),
                          coordinates_t(2,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetNeigbour(Direction::West),
                          coordinates_t(1,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetNeigbour(Direction::South),
                          coordinates_t(2,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetNeigbour(Direction::East),
                          coordinates_t(4,1));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetNeigbour(Direction::North),
                          coordinates_t(3,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetNeigbour(Direction::West),
                          coordinates_t(1,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetNeigbour(Direction::South),
                          coordinates_t(3,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetNeigbour(Direction::East),
                          coordinates_t(4,1));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetNeigbour(Direction::North),
                          coordinates_t(4,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetNeigbour(Direction::West),
                          coordinates_t(1,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetNeigbour(Direction::South),
                          coordinates_t(4,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetNeigbour(Direction::East),
                          coordinates_t(4,1));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetNeigbour(Direction::North),
                          coordinates_t(1,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetNeigbour(Direction::West),
                          coordinates_t(1,2));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetNeigbour(Direction::South),
                          coordinates_t(1,2));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetNeigbour(Direction::East),
                          coordinates_t(3,2));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetNeigbour(Direction::North),
                          coordinates_t(2,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetNeigbour(Direction::West),
                          coordinates_t(1,2));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetNeigbour(Direction::South),
                          coordinates_t(2,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetNeigbour(Direction::East),
                          coordinates_t(3,2));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetNeigbour(Direction::North),
                          coordinates_t(3,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetNeigbour(Direction::West),
                          coordinates_t(1,2));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetNeigbour(Direction::South),
                          coordinates_t(3,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetNeigbour(Direction::East),
                          coordinates_t(3,2));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetNeigbour(Direction::North),
                          coordinates_t(4,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetNeigbour(Direction::West),
                          coordinates_t(4,2));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetNeigbour(Direction::South),
                          coordinates_t(4,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetNeigbour(Direction::East),
                          coordinates_t(4,2));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetNeigbour(Direction::North),
                          coordinates_t(1,3));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetNeigbour(Direction::West),
                          coordinates_t(1,3));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetNeigbour(Direction::South),
                          coordinates_t(1,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetNeigbour(Direction::East),
                          coordinates_t(4,3));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetNeigbour(Direction::North),
                          coordinates_t(2,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetNeigbour(Direction::West),
                          coordinates_t(1,3));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetNeigbour(Direction::South),
                          coordinates_t(2,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetNeigbour(Direction::East),
                          coordinates_t(4,3));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetNeigbour(Direction::North),
                          coordinates_t(3,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetNeigbour(Direction::West),
                          coordinates_t(1,3));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetNeigbour(Direction::South),
                          coordinates_t(3,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetNeigbour(Direction::East),
                          coordinates_t(4,3));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetNeigbour(Direction::North),
                          coordinates_t(4,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetNeigbour(Direction::West),
                          coordinates_t(1,3));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetNeigbour(Direction::South),
                          coordinates_t(4,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetNeigbour(Direction::East),
                          coordinates_t(4,3));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetNeigbour(Direction::North),
                          coordinates_t(1,3));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetNeigbour(Direction::West),
                          coordinates_t(1,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetNeigbour(Direction::South),
                          coordinates_t(1,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetNeigbour(Direction::East),
                          coordinates_t(4,4));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetNeigbour(Direction::North),
                          coordinates_t(2,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetNeigbour(Direction::West),
                          coordinates_t(1,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetNeigbour(Direction::South),
                          coordinates_t(2,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetNeigbour(Direction::East),
                          coordinates_t(4,4));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetNeigbour(Direction::North),
                          coordinates_t(3,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetNeigbour(Direction::West),
                          coordinates_t(1,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetNeigbour(Direction::South),
                          coordinates_t(3,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetNeigbour(Direction::East),
                          coordinates_t(4,4));

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetNeigbour(Direction::North),
                          coordinates_t(4,1));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetNeigbour(Direction::West),
                          coordinates_t(1,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetNeigbour(Direction::South),
                          coordinates_t(4,4));
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetNeigbour(Direction::East),
                          coordinates_t(4,4));

        // then check connected holes
        std::vector<coordinates_t> hole_1 = { coordinates_t(1,1) };
        std::vector<coordinates_t> hole_2 = { coordinates_t(4,3) };
        std::vector<coordinates_t> null_vector = { };


        // First check connections between cells
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,1)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetHolesOnWayTo(Direction::West),
                          hole_1);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,1)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetHolesOnWayTo(Direction::West),
                          hole_1);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,1)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetHolesOnWayTo(Direction::West),
                          hole_1);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetHolesOnWayTo(Direction::South),
                          hole_2);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,1)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetHolesOnWayTo(Direction::North),
                          hole_1);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,2)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,2)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,2)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetHolesOnWayTo(Direction::South),
                          hole_2);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,2)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,3)).GetHolesOnWayTo(Direction::East),
                          hole_2);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,3)).GetHolesOnWayTo(Direction::East),
                          hole_2);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,3)).GetHolesOnWayTo(Direction::East),
                          hole_2);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,3)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(1,4)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(2,4)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetHolesOnWayTo(Direction::North),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(3,4)).GetHolesOnWayTo(Direction::East),
                          null_vector);

        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetHolesOnWayTo(Direction::North),
                          hole_2);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetHolesOnWayTo(Direction::West),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetHolesOnWayTo(Direction::South),
                          null_vector);
        BOOST_CHECK_EQUAL(move_graph_.at(coordinates_t(4,4)).GetHolesOnWayTo(Direction::East),
                          null_vector);
    }


};

BOOST_AUTO_TEST_CASE( check_table )
{
    CheckTable t;
    t.CalculateMoves();
    t.CkeckMoveGraph();
}
