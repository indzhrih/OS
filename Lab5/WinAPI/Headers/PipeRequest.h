#ifndef LAB5_PIPEREQUEST_H
#define LAB5_PIPEREQUEST_H

#include "PipeRequestType.h"
#include "Employee.h"

struct PipeRequest {
    PipeRequestType type;
    int employeeId;
    employee employeeData;

    PipeRequest();
};

#endif
