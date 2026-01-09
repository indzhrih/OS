#ifndef LAB5_EMPLOYEESTORAGE_H
#define LAB5_EMPLOYEESTORAGE_H

#include <string>
#include "Employee.h"

class EmployeeStorage {
public:
    EmployeeStorage(const std::string& filePathValue);

    void createFromConsole();
    void printAll() const;
    bool readEmployee(int employeeId, employee& result) const;
    bool writeEmployee(int employeeId, const employee& updatedEmployee) const;
    const std::string& getFilePath() const;

private:
    std::string filePath;
};

#endif
