#include "../Headers/Employee.h"

employee::employee() {
    num = 0;
    for (int i = 0; i < 10; ++i) name[i] = 0;
    hours = 0.0;
}
