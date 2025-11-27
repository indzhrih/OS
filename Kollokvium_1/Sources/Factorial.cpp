#include "../Headers/Factorial.h"
#include <iostream>

Factorial::Factorial(int n) : n(n) {
    factorials.resize(n);
    if (n > 0) {
        factorials[0] = 1;
        for (int i = 1; i < n; i++) factorials[i] = factorials[i - 1] * (i + 1);
    }
    else std::cout << "Factorial can't be negative!" << std::endl;
}

void Factorial::printFactorials() {
    for (unsigned long long i : factorials) std::cout << i << " ";
    std::cout << std::endl;
}
