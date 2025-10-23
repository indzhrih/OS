#ifndef KOLLOKVIUM_1_FACTORIAL_H
#define KOLLOKVIUM_1_FACTORIAL_H

#include <vector>

class Factorial {
public:
    Factorial(int n);

    void printFactorials();
private:
    std::vector<unsigned long long> factorials;
    int n;
};

#endif //KOLLOKVIUM_1_FACTORIAL_H
