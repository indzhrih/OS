#include <gtest/gtest.h>
#include "../Headers/Factorial.h"

TEST(FactorialTest, ConstructorWithPositiveNumber_CreatesValidObject) {
    Factorial factorial(5);
    SUCCEED();
}

TEST(FactorialTest, PrintFactorialsWithFive_OutputsCorrectSequence) {
    Factorial factorial(5);
    testing::internal::CaptureStdout();
    factorial.printFactorials();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 6 24 120 \n");
}

TEST(FactorialTest, PrintFactorialsWithZero_OutputsEmptyLine) {
    Factorial factorial(0);
    testing::internal::CaptureStdout();
    factorial.printFactorials();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "\n");
}

TEST(FactorialTest, PrintFactorialsWithOne_OutputsSingleFactorial) {
    Factorial factorial(1);
    testing::internal::CaptureStdout();
    factorial.printFactorials();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 \n");
}
