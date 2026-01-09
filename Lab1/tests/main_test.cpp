#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <filesystem>

static bool exists(const char* p){ std::ifstream f(p); return f.good(); }
static std::string read(const char* p){ std::ifstream f(p); std::stringstream s; s<<f.rdbuf(); return s.str(); }

class TestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        std::filesystem::create_directories("tests_bin");
    }
};

TEST(CreatorTest, CreatesValidBinary) {
    int r = std::system("Creator.exe tests_bin/ok.bin 2 < tests/input_data/input_creator_valid.txt");
    EXPECT_EQ(r, 0);
    EXPECT_TRUE(exists("tests_bin/ok.bin"));
}

TEST(CreatorTest, DuplicateEmployeeNumber) {
    int r = std::system("Creator.exe tests_bin/dup.bin 2 < tests/input_data/input_creator_dup.txt");
    EXPECT_NE(r, 0);
}

TEST(CreatorTest, TooLongNameTruncated) {
    int r = std::system("Creator.exe tests_bin/long.bin 1 < tests/input_data/input_creator_longname.txt");
    EXPECT_EQ(r, 0);
    EXPECT_TRUE(exists("tests_bin/long.bin"));
}

TEST(CreatorTest, InvalidRecordsCount) {
    int r = std::system("Creator.exe");
    EXPECT_NE(r, 0);
}

TEST(CreatorTest, InvalidNumInput) {
    std::ofstream bad("tests/input_data/tmp_bad_num.txt");
    bad << "A Name 5\n";
    bad.close();
    int r = std::system("Creator.exe tests_bin/badnum.bin 1 < tests/input_data/tmp_bad_num.txt");
    EXPECT_NE(r, 0);
}

TEST(CreatorTest, InvalidHoursInput) {
    std::ofstream bad("tests/input_data/tmp_bad_hours.txt");
    bad << "3 Bob Zzz\n";
    bad.close();
    int r = std::system("Creator.exe tests_bin/badhours.bin 1 < tests/input_data/tmp_bad_hours.txt");
    EXPECT_NE(r, 0);
}

TEST(ReporterTest, GeneratesReport) {
    std::system("Creator.exe tests_bin/r_ok.bin 2 < tests/input_data/input_creator_valid.txt");
    int r = std::system("Reporter.exe tests_bin/r_ok.bin tests_bin/report.txt 10");
    EXPECT_EQ(r, 0);
    EXPECT_TRUE(exists("tests_bin/report.txt"));
    auto c = read("tests_bin/report.txt");
    EXPECT_NE(c.find("Report"), std::string::npos);
    EXPECT_NE(c.find("Salary"), std::string::npos);
}

TEST(ReporterTest, NegativeSalaryFails) {
    std::system("Creator.exe tests_bin/r_neg.bin 1 < tests/input_data/input_creator_valid.txt");
    int r = std::system("Reporter.exe tests_bin/r_neg.bin tests_bin/r_neg.txt -5 --test-mode");
    EXPECT_NE(r, 0);
}

TEST(ReporterTest, SalaryNotNumberFails) {
    std::system("Creator.exe tests_bin/r_nan.bin 1 < tests/input_data/input_creator_valid.txt");
    int r = std::system("Reporter.exe tests_bin/r_nan.bin tests_bin/r_nan.txt abc --test-mode");
    EXPECT_NE(r, 0);
}

TEST(ReporterTest, MissingBinaryFileFails) {
    int r = std::system("Reporter.exe tests_bin/no_such.bin tests_bin/out.txt 10 --test-mode");
    EXPECT_NE(r, 0);
}

TEST(ReporterTest, UnwritableOutputPathFails) {
    std::system("Creator.exe tests_bin/r_path.bin 1 < tests/input_data/input_creator_valid.txt");
    int r = std::system("Reporter.exe tests_bin/r_path.bin tests_bin/no_dir/out.txt 10 --test-mode");
    EXPECT_NE(r, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnvironment());
    return RUN_ALL_TESTS();
}
