#include "../Headers/Employee.h"
#include <iostream>

employee::employee() {
    num = 0;
    for (int i = 0; i < 10; ++i) name[i] = 0;
    hours = 0.0;
}

void employee::printInfo() const {
    std::cout << "num = " << num << ", name = " << name << ", hours = " << hours << std::endl;
}
