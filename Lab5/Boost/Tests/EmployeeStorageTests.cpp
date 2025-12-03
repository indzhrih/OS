#include <boost/test/unit_test.hpp>
#include "../Headers/EmployeeStorage.h"
#include "../Headers/Employee.h"
#include "../Headers/ExceptionHandler.h"
#include <cstdio>
#include <sstream>

static std::string generateTempFilePath(const std::string& suffix) {
    return "employee_storage_test_" + suffix + ".bin";
}

static void writeSingleEmployee(const std::string& path, int num, const std::string& name, double hours) {
    FILE* file = std::fopen(path.c_str(), "wb");
    if (!file) return;
    employee record;
    record.num = num;
    for (int i = 0; i < 10; ++i) record.name[i] = 0;
    for (size_t i = 0; i < name.size() && i < 9; ++i) record.name[i] = name[i];
    record.hours = hours;
    std::fwrite(&record, sizeof(employee), 1, file);
    std::fclose(file);
}

BOOST_AUTO_TEST_SUITE(EmployeeStorageTests)

BOOST_AUTO_TEST_CASE(ConstructorStoresFilePath) {
    std::string path = "employees_test.bin";
    EmployeeStorage storage(path);
    bool result = storage.getFilePath() == path;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(CreateFromConsoleCreatesCorrectNumberOfRecords) {
    std::string path = generateTempFilePath("create_from_console");
    EmployeeStorage storage(path);

    std::streambuf* originalBuffer = std::cin.rdbuf();
    std::istringstream input("2\n1\nAnn\n10\n2\nBob\n20\n");
    std::cin.rdbuf(input.rdbuf());

    storage.createFromConsole();

    std::cin.rdbuf(originalBuffer);
    std::cin.clear();

    FILE* file = std::fopen(path.c_str(), "rb");
    int count = 0;
    if (file) {
        employee record;
        while (std::fread(&record, sizeof(employee), 1, file) == 1) ++count;
        std::fclose(file);
    }
    std::remove(path.c_str());
    bool result = count == 2;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(PrintAllOutputsEmployeesFromFile) {
    std::string path = generateTempFilePath("print_all");
    writeSingleEmployee(path, 7, "Alice", 15.0);

    EmployeeStorage storage(path);

    std::streambuf* originalBuffer = std::cout.rdbuf();
    std::ostringstream output;
    std::cout.rdbuf(output.rdbuf());

    storage.printAll();

    std::cout.rdbuf(originalBuffer);
    std::string text = output.str();
    std::remove(path.c_str());

    bool result = text.find("num = 7") != std::string::npos && text.find("Alice") != std::string::npos;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ReadEmployeeReturnsTrueForExistingEmployee) {
    std::string path = generateTempFilePath("read_existing");
    writeSingleEmployee(path, 3, "Tom", 8.0);

    EmployeeStorage storage(path);
    employee record;
    bool found = storage.readEmployee(3, record);
    std::remove(path.c_str());

    bool result = found && record.num == 3 && record.hours == 8.0;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ReadEmployeeReturnsFalseForMissingEmployee) {
    std::string path = generateTempFilePath("read_missing");
    writeSingleEmployee(path, 4, "Sam", 5.0);

    EmployeeStorage storage(path);
    employee record;
    bool found = storage.readEmployee(10, record);
    std::remove(path.c_str());

    bool result = !found;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(WriteEmployeeUpdatesExistingRecord) {
    std::string path = generateTempFilePath("write_existing");
    writeSingleEmployee(path, 9, "Max", 6.0);

    EmployeeStorage storage(path);
    employee updated;
    updated.num = 9;
    for (int i = 0; i < 10; ++i) updated.name[i] = 0;
    updated.name[0] = 'N';
    updated.name[1] = 'e';
    updated.name[2] = 'w';
    updated.hours = 20.0;

    bool written = storage.writeEmployee(9, updated);

    employee readBack;
    bool found = storage.readEmployee(9, readBack);
    std::remove(path.c_str());

    bool result = written && found && readBack.hours == 20.0 && readBack.name[0] == 'N';
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(WriteEmployeeReturnsFalseForMissingRecord) {
    std::string path = generateTempFilePath("write_missing");
    writeSingleEmployee(path, 1, "One", 3.0);

    EmployeeStorage storage(path);
    employee updated;
    updated.num = 2;
    for (int i = 0; i < 10; ++i) updated.name[i] = 0;
    updated.hours = 7.0;

    bool written = storage.writeEmployee(2, updated);
    std::remove(path.c_str());

    bool result = !written;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
