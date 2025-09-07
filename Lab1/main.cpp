#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include "Employee.h"

void printBinaryFile(const std::string& path) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) throw std::runtime_error("Cannot open binary file: " + path);

    std::cout << "Binary file content " << path << std::endl;
    employee emp;
    while (std::fread(&emp, sizeof(employee), 1, f) == 1) {
        std::cout << "num = " << emp.num << ", name = " << emp.name << ", hours = " << emp.hours << "\n";
    }
    std::fclose(f);
    std::cout << "-----------------------------------------------\n";
}

void printReportFile(const std::string& path) {
    std::ifstream fin(path);
    std::string line;
    if (!fin) throw std::runtime_error("Cannot open report file: " + path);

    std::cout << "Report " << path << "\n";
    while (std::getline(fin, line)) std::cout << line << '\n';
}

void creatorProcess(std::string &binaryFilename) {
    STARTUPINFO siCreator;
    PROCESS_INFORMATION piCreator;
    ZeroMemory(&siCreator, sizeof(STARTUPINFO));
    siCreator.cb = sizeof(STARTUPINFO);

    std::string creatorCmd;
    int empNumber;

    try {
        std::cout << "Enter binary file name " << std::endl;
        std::getline(std::cin >> std::ws, binaryFilename);

        std::cout << "Enter number of records " << std::endl;
        if (!(std::cin >> empNumber) || empNumber <= 0) {
            throw std::runtime_error("recordsCount must be a positive integer");
        }

        creatorCmd = "Creator.exe " + binaryFilename + " " + std::to_string(empNumber);
        std::cout << "Starting: " << creatorCmd << std::endl;

        if (!CreateProcess(NULL, const_cast<LPSTR>(creatorCmd.c_str()), NULL, NULL, FALSE,
                           CREATE_NEW_CONSOLE, 0, NULL, &siCreator, &piCreator))
        {
            DWORD error = GetLastError();
            throw std::runtime_error("CreateProcess(Creator) failed! code=" + std::to_string(error));
        }

        WaitForSingleObject(piCreator.hProcess, INFINITE);
        CloseHandle(piCreator.hProcess);
        CloseHandle(piCreator.hThread);

        printBinaryFile(binaryFilename);

    }
    catch (const std::exception& e) {
        std::cerr << "creatorProcess error: " << e.what() << std::endl;
        std::cerr << "Press any key to close this window...";
        system("pause");
    }
}

void reporterProcess(const std::string &binaryFilename) {
    STARTUPINFO siReporter;
    PROCESS_INFORMATION piReporter;
    ZeroMemory(&siReporter, sizeof(STARTUPINFO));
    siReporter.cb = sizeof(STARTUPINFO);

    std::string reporterCmd;
    std::string reportFilename;
    double salary;

    try {
        std::cout << "Enter reporter file name " << std::endl;
        std::getline(std::cin >> std::ws, reportFilename);

        std::cout << "Enter salary " << std::endl;
        if (!(std::cin >> salary) || !(salary > 0.0)) {
            throw std::runtime_error("salary (hourly rate) must be a positive number");
        }

        reporterCmd = "Reporter.exe " + binaryFilename + " " + reportFilename + " " + std::to_string(salary);
        std::cout << "Starting: " << reporterCmd << std::endl;

        if (!CreateProcess(NULL, const_cast<LPSTR>(reporterCmd.c_str()), NULL, NULL, FALSE,
                           CREATE_NEW_CONSOLE, 0, NULL, &siReporter, &piReporter))
        {
            DWORD error = GetLastError();
            throw std::runtime_error("CreateProcess(Reporter) failed! code=" + std::to_string(error));
        }

        WaitForSingleObject(piReporter.hProcess, INFINITE);
        CloseHandle(piReporter.hProcess);
        CloseHandle(piReporter.hThread);

        printReportFile(reportFilename);

    }
    catch (const std::exception& e) {
        std::cerr << "reporterProcess error: " << e.what() << std::endl;
        std::cerr << "Press any key to close this window...";
        system("pause");
    }
}

int main() {
    try {
        std::string binaryFilename;
        creatorProcess(binaryFilename);
        reporterProcess(binaryFilename);
    }
    catch (const std::exception& e) {
        std::cerr << "Main error: " << e.what() << std::endl;
        std::cerr << "Press any key to close this window...";
        system("pause");
    }

    std::cout << "Program finished work" << std::endl;
    std::cout << "Press any key to exit" << std::endl;
    system("pause");
}
