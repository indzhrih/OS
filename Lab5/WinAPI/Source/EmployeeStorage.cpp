#include "../Headers/EmployeeStorage.h"
#include "../Headers/ExceptionHandler.h"
#include <iostream>
#include <cstdio>
#include <set>
#include <limits>
#include <cstring>
#include <stdexcept>

static int readPositiveIntFromConsole(const std::string& prompt) {
    int value;
    std::cout << prompt << std::endl;
    while (!(std::cin >> value) || value <= 0) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter positive integer" << std::endl;
    }
    return value;
}

static double readDoubleFromConsole(const std::string& prompt) {
    double value;
    std::cout << prompt << std::endl;
    while (!(std::cin >> value)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter number" << std::endl;
    }
    return value;
}

static void readStringFromConsole(const std::string& prompt, std::string& result) {
    std::cout << prompt << std::endl;
    while (!(std::cin >> result)) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter text" << std::endl;
    }
}

EmployeeStorage::EmployeeStorage(const std::string& filePathValue) : filePath(filePathValue) {
}

void EmployeeStorage::createFromConsole() {
    int employeesCount = readPositiveIntFromConsole("Enter number of employees");

    FILE* file = std::fopen(filePath.c_str(), "wb");
    if (!ExceptionHandler::checkFile(file, "Create binary file")) throw std::runtime_error("Cannot create file " + filePath);

    std::set<int> usedIds;

    for (int i = 0; i < employeesCount; ++i) {
        employee record;
        std::string nameText;

        std::cout << "Employee " << i + 1 << std::endl;

        std::cout << "Enter employee number" << std::endl;
        while (!(std::cin >> record.num)) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input! Enter integer number" << std::endl;
        }

        if (!usedIds.insert(record.num).second) {
            std::fclose(file);
            throw std::runtime_error("Duplicate employee number " + std::to_string(record.num));
        }

        readStringFromConsole("Enter employee name", nameText);
        if (nameText.size() > 9) nameText = nameText.substr(0, 9);
        std::memset(record.name, 0, sizeof(record.name));
        std::memcpy(record.name, nameText.c_str(), nameText.size());

        record.hours = readDoubleFromConsole("Enter worked hours");

        size_t written = std::fwrite(&record, sizeof(employee), 1, file);
        if (written != 1) {
            std::fclose(file);
            throw std::runtime_error("Write to file failed");
        }
    }

    std::fclose(file);
}

void EmployeeStorage::printAll() const {
    FILE* file = std::fopen(filePath.c_str(), "rb");
    if (!ExceptionHandler::checkFile(file, "Open binary file for reading")) throw std::runtime_error("Cannot open file " + filePath);

    std::cout << "Employees in file " << filePath << std::endl;

    employee record;
    while (std::fread(&record, sizeof(employee), 1, file) == 1) record.printInfo();

    std::fclose(file);
}

bool EmployeeStorage::readEmployee(int employeeId, employee& result) const {
    FILE* file = std::fopen(filePath.c_str(), "rb");
    if (!ExceptionHandler::checkFile(file, "Open file for readEmployee")) throw std::runtime_error("Cannot open file " + filePath);

    employee record;
    bool found = false;

    while (std::fread(&record, sizeof(employee), 1, file) == 1) {
        if (record.num == employeeId) {
            result = record;
            found = true;
            break;
        }
    }

    std::fclose(file);
    return found;
}

bool EmployeeStorage::writeEmployee(int employeeId, const employee& updatedEmployee) const {
    FILE* file = std::fopen(filePath.c_str(), "rb+");
    if (!ExceptionHandler::checkFile(file, "Open file for writeEmployee")) {
        throw std::runtime_error("Cannot open file " + filePath);
    }

    employee record;
    bool updated = false;

    while (std::fread(&record, sizeof(employee), 1, file) == 1) {
        if (record.num == employeeId) {
            long position = -static_cast<long>(sizeof(employee));
            if (std::fseek(file, position, SEEK_CUR) != 0) {
                std::fclose(file);
                throw std::runtime_error("fseek failed in writeEmployee");
            }

            size_t written = std::fwrite(&updatedEmployee, sizeof(employee), 1, file);
            if (written != 1) {
                std::fclose(file);
                throw std::runtime_error("fwrite failed in writeEmployee");
            }

            updated = true;
            break;
        }
    }

    std::fclose(file);
    return updated;
}

const std::string& EmployeeStorage::getFilePath() const {
    return filePath;
}
