#include <gtest/gtest.h>
#include "../Headers/Menu.h"
#include <iostream>
#include <sstream>

TEST(MenuTest, MenuShowsOptionsAndExits) {
    Menu menu;

    std::stringstream testInput;
    testInput << "0\n";

    std::streambuf* originalCin = std::cin.rdbuf();
    std::cin.rdbuf(testInput.rdbuf());

    testing::internal::CaptureStdout();
    menu.openMenu();
    std::string output = testing::internal::GetCapturedStdout();

    std::cin.rdbuf(originalCin);

    EXPECT_TRUE(output.find("Choose task:") != std::string::npos);
}
