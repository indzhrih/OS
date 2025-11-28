#ifndef LAB5_EMPLOYEEFILE_H
#define LAB5_EMPLOYEEFILE_H

#include <string>
#include "Employee.h"

class EmployeeFile {
public:
    EmployeeFile(const std::string& filePath);

    void createFromConsole();
    void printAll() const;
    bool readEmployee(int employeeId, employee& result) const;
    bool writeEmployee(int employeeId, const employee& updatedEmployee) const;
    const std::string& getFilePath() const;

private:
    std::string filePath;
};

#endif
