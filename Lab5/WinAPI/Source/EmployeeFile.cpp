#include "../Headers/EmployeeFile.h"
#include "../Headers/ExceptionHandler.h"
#include <iostream>
#include <cstdio>
#include <set>
#include <limits>
#include <cstring>
#include <stdexcept>

EmployeeFile::EmployeeFile(const std::string& filePathValue) : filePath(filePathValue) {
}

void EmployeeFile::createFromConsole() {
    int employeesCount;
    std::cout << "Enter number of employees" << std::endl;
    while (!(std::cin >> employeesCount) || employeesCount <= 0) {
        ExceptionHandler::clearInput();
        std::cout << "Wrong input! Enter positive integer" << std::endl;
    }

    FILE* file = std::fopen(filePath.c_str(), "wb");
    if (!ExceptionHandler::checkFile(file, "create file")) {
        throw std::runtime_error("Cannot create file " + filePath);
    }

    std::set<int> usedIds;

    for (int i = 0; i < employeesCount; ++i) {
        employee record;
        std::string nameText;

        std::cout << "Employee " << i + 1 << " number" << std::endl;
        while (!(std::cin >> record.num)) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input! Enter integer number" << std::endl;
        }

        if (!usedIds.insert(record.num).second) {
            std::fclose(file);
            throw std::runtime_error("Duplicate employee number " + std::to_string(record.num));
        }

        std::cout << "Employee " << i + 1 << " name" << std::endl;
        while (!(std::cin >> nameText)) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input! Enter name" << std::endl;
        }

        if (nameText.size() > 9) nameText = nameText.substr(0, 9);
        std::memset(record.name, 0, sizeof(record.name));
        std::memcpy(record.name, nameText.c_str(), nameText.size());

        std::cout << "Employee " << i + 1 << " hours" << std::endl;
        while (!(std::cin >> record.hours)) {
            ExceptionHandler::clearInput();
            std::cout << "Wrong input! Enter hours" << std::endl;
        }

        size_t written = std::fwrite(&record, sizeof(employee), 1, file);
        if (written != 1) {
            std::fclose(file);
            throw std::runtime_error("Write to file failed");
        }
    }

    std::fclose(file);
}

void EmployeeFile::printAll() const {
    FILE* file = std::fopen(filePath.c_str(), "rb");
    if (!ExceptionHandler::checkFile(file, "open file for reading")) {
        throw std::runtime_error("Cannot open file " + filePath);
    }

    std::cout << "Employees in file " << filePath << std::endl;

    employee record;
    while (std::fread(&record, sizeof(employee), 1, file) == 1) {
        std::cout << "num = " << record.num
                  << ", name = " << record.name
                  << ", hours = " << record.hours << std::endl;
    }

    std::fclose(file);
}

bool EmployeeFile::readEmployee(int employeeId, employee& result) const {
    FILE* file = std::fopen(filePath.c_str(), "rb");
    if (!ExceptionHandler::checkFile(file, "open file for readEmployee")) {
        throw std::runtime_error("Cannot open file " + filePath);
    }

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

bool EmployeeFile::writeEmployee(int employeeId, const employee& updatedEmployee) const {
    FILE* file = std::fopen(filePath.c_str(), "rb+");
    if (!ExceptionHandler::checkFile(file, "open file for writeEmployee")) {
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

const std::string& EmployeeFile::getFilePath() const {
    return filePath;
}
