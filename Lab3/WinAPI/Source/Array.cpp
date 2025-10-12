#include <iostream>
#include <stdexcept>
#include "../Headers/Array.h"

Array::Array() {
    size = 0;
}

Array::Array(int size) : size(size) {
    if (size <= 0) {
        throw std::invalid_argument("Array size must be positive");
    }
    array = new int[size];
    for (int i = 0; i < size; i++) array[i] = 0;
}

Array::~Array() {
    delete[] array;
}

void Array::printArray() {
    checkArrayInitialization();
    std::cout << "Array: ";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void Array::checkArrayInitialization() {
    if (size <= 0 || array == nullptr) {
        throw std::out_of_range("Cannot switch elements in an empty or uninitialized array.");
    }
}

int& Array::operator[](int index) {
    if (index >= size || index < 0) {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}