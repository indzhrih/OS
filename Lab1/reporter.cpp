#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Employee.h"

int main(int argc, char* argv[]) {
    bool isTestMode = false;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--test-mode") {
            isTestMode = true;
            break;
        }
    }

    if (argc < 4) {
        std::cerr << "Not enough args!";
        if (!isTestMode) system("pause");
        return 1;
    }
    char* binaryFileName = argv[1];
    char* outputFileName = argv[2];
    employee emp;
    double employeeSalary = 0;
    double salary;

    try {
        try { salary = std::stod(argv[3]); }
        catch (...) { throw std::runtime_error("Salary must be a valid number"); }
        if (salary < 0.0) throw std::runtime_error("Salary must be positive");

        FILE* binaryFile = std::fopen(binaryFileName, "rb");
        if (!binaryFile) {
            throw std::runtime_error(std::string("Error opening binary file: ") + binaryFileName);
        }

        std::ofstream outFile(outputFileName);
        if (!outFile) {
            throw std::runtime_error(std::string("Error creating output file: ") + outputFileName);
        }

        outFile << "Report based on  \"" << binaryFileName << "\"" << std::endl;
        outFile << std::setw(10) << "Number"
                << std::setw(15) << "Name"
                << std::setw(10) << "Hours"
                << std::setw(15) << "Salary"
                << std::endl;
        outFile << "---------------------------------------------------" << std::endl;

        while (fread(&emp, sizeof(employee), 1, binaryFile) == 1) {
            employeeSalary = emp.hours * salary;

            outFile << std::setw(10) << emp.num
                    << std::setw(15) << emp.name
                    << std::setw(10) << std::fixed << std::setprecision(2) << emp.hours
                    << std::setw(15) << std::fixed << std::setprecision(2) << employeeSalary
                    << std::endl;

        }
        outFile << "---------------------------------------------------" << std::endl;

        fclose(binaryFile);
        outFile.close();
    }
    catch (const std::exception& ex) {
        std::cerr << "Reporter error: " << ex.what() << std::endl;
        if (!isTestMode) {
            std::cerr << "Press any key to close this window...";
            system("pause");
        }
        return 1;
    }

    return 0;
}
