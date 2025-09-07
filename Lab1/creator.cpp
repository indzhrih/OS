#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include "Employee.h"

int main(int argc, char* argv[]) {
    int lineCount = std::atoi(argv[2]);
    std::string binaryFileName = argv[1];

    std::set<int> usedNums;
    std::string name;
    employee emp;
    FILE* out;

    try {
        out = fopen(binaryFileName.c_str(), "wb");
        if (!out) throw std::runtime_error("Failed to open output file: " + binaryFileName);

        std::cout << "Input code, name and hours" << std::endl;
        for (int i = 0; i < lineCount; i++) {
            std::cout << "Employee number " << i + 1 << "\n";

            if (!(std::cin >> emp.num)) {
                std::fclose(out);
                throw std::runtime_error("Invalid input for employee.num");
            }

            if (!usedNums.insert(emp.num).second) {
                std::fclose(out);
                throw std::runtime_error("Duplicate employee number: " + std::to_string(emp.num));
            }

            std::string name;
            if (!(std::cin >> name)) {
                std::fclose(out);
                throw std::runtime_error("Invalid input for employee.name");
            }

            if (name.size() > 9) {
                std::cout << "Name too long, will be truncated to 9 chars.\n";
                name = name.substr(0, 9);
            }
            std::strncpy(emp.name, name.c_str(), 9);
            emp.name[9] = '\0';

            if (!(std::cin >> emp.hours)) {
                std::fclose(out);
                throw std::runtime_error("Invalid input for employee.hours");
            }

            if (std::fwrite(&emp, sizeof(employee), 1, out) != 1) {
                std::fclose(out);
                throw std::runtime_error("fwrite failed");
            }
        }

        fclose(out);
    }
    catch (const std::exception& ex) {
        std::cerr << "Creator error: " << ex.what() << "\n";
        std::cerr << "Press any key to close this window...";
        system("pause");
        return 1;
    }
    return 0;
}
