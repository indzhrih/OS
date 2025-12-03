#include <boost/test/unit_test.hpp>
#include "../Headers/PipeResponse.h"
#include "../Headers/PipeResponseStatus.h"

BOOST_AUTO_TEST_SUITE(PipeResponseTests)

BOOST_AUTO_TEST_CASE(DefaultConstructorInitializesFields) {
    PipeResponse response;
    bool nameZero = true;
    for (int i = 0; i < 10; ++i) if (response.employeeData.name[i] != 0) { nameZero = false; break; }
    bool result = response.status == PIPE_RESPONSE_ERROR &&
                  response.employeeData.num == 0 &&
                  response.employeeData.hours == 0.0 &&
                  nameZero;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
