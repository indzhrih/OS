#ifndef LAB5_PIPERESPONSE_H
#define LAB5_PIPERESPONSE_H

#include "PipeResponseStatus.h"
#include "Employee.h"

class PipeResponse {
public:
    PipeResponse();

    PipeResponseStatus status;
    employee employeeData;
};

#endif
