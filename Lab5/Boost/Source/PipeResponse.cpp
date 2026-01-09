#include "../Headers/PipeResponse.h"

PipeResponse::PipeResponse() {
    status = PIPE_RESPONSE_ERROR;
    employeeData = employee();
}
