#include <boost/test/unit_test.hpp>
#include "../Headers/PipeRequest.h"
#include "../Headers/PipeRequestType.h"

BOOST_AUTO_TEST_SUITE(PipeRequestTests)

BOOST_AUTO_TEST_CASE(DefaultConstructorInitializesFields) {
    PipeRequest request;
    bool nameZero = true;
    for (int i = 0; i < 10; ++i) if (request.employeeData.name[i] != 0) { nameZero = false; break; }
    bool result = request.type == PIPE_REQUEST_READ &&
                  request.employeeId == 0 &&
                  request.employeeData.num == 0 &&
                  request.employeeData.hours == 0.0 &&
                  nameZero;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
