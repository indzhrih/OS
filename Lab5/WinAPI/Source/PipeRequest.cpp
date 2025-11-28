#include "../Headers/PipeRequest.h"

PipeRequest::PipeRequest() {
    type = PIPE_REQUEST_READ;
    employeeId = 0;
    employeeData = employee();
}
