#ifndef LAB5_PIPEREQUEST_H
#define LAB5_PIPEREQUEST_H

#include "PipeRequestType.h"
#include "Employee.h"

class PipeRequest {
public:
    PipeRequest();

    PipeRequestType type;
    int employeeId;
    employee employeeData;
};

#endif
