#ifndef LAB5_PIPERESPONSE_H
#define LAB5_PIPERESPONSE_H

#include "PipeResponseStatus.h"
#include "Employee.h"

struct PipeResponse {
    PipeResponseStatus status;
    employee employeeData;

    PipeResponse();
};

#endif
