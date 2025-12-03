#include <boost/test/unit_test.hpp>
#include "../Headers/Employee.h"
#include <sstream>
#include <iostream>

BOOST_AUTO_TEST_SUITE(EmployeeTests)

BOOST_AUTO_TEST_CASE(DefaultConstructorInitializesAllFieldsToZero) {
    employee e;
    bool nameZero = true;
    for (int i = 0; i < 10; ++i) if (e.name[i] != 0) { nameZero = false; break; }
    bool result = e.num == 0 && e.hours == 0.0 && nameZero;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(PrintInfoOutputsEmployeeData) {
    employee e;
    e.num = 5;
    e.hours = 12.5;
    for (int i = 0; i < 10; ++i) e.name[i] = 0;
    e.name[0] = 'B';
    e.name[1] = 'o';
    e.name[2] = 'b';

    std::streambuf* originalBuffer = std::cout.rdbuf();
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());

    e.printInfo();

    std::cout.rdbuf(originalBuffer);
    std::string text = output.str();
    bool result = text.find("num = 5") != std::string::npos && text.find("Bob") != std::string::npos;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
