 #define BOOST_TEST_DYN_LINK
 #define BOOST_TEST_MODULE "TG_file_ops"

#include <boost/test/unit_test.hpp>

#include "file_ops.h"
#include "tests_config.h"


BOOST_AUTO_TEST_CASE( input_data )
{
    std::string sample_file (SAMPLE_FILE);

    FileInput in (sample_file);
    const input_data_t & data = in.GetData();

    BOOST_CHECK_EQUAL_COLLECTIONS(sample.begin(), sample.end(), data.begin(), data.end());

}
