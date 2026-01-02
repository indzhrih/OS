#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include "../Headers/Array.h"

Array::Array() {
    size = 0;
}

Array::Array(int size) : size(size) {
    if (size <= 0) {
        throw std::invalid_argument("Array size must be positive");
    }
    array = new int[size];
}

Array::~Array() {
    delete[] array;
}

void Array::inputArray() {
    checkArrayInitialization();

    std::cout << "Input " << size << " numbers: " << std::endl;
    for (int i = 0; i < size; i++) {
        while (true) {
            if (std::cin >> array[i]) break;

            std::cerr << "Input error, try again: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
}

void Array::printArray() {
    checkArrayInitialization();

    std::cout << "Array: ";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void Array::switchMaxAndMinOnAverage() {
    checkArrayInitialization();

    array[maxElementIndex] = averageValue;
    array[minElementIndex] = averageValue;
}

void Array::checkArrayInitialization() {
    if (size <= 0 || array == nullptr) {
        throw std::out_of_range("Cannot switch elements in an empty or uninitialized array.");
    }
}


int Array::findMaxElement() {
    checkArrayInitialization();

    int max = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
            maxElementIndex = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    return max;
}

int Array::findMinElement() {
    checkArrayInitialization();

    int min = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min) {
            min = array[i];
            minElementIndex = i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    return min;
}

int Array::calculateAverageValue() {
    checkArrayInitialization();

    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
    averageValue = sum / size;

    return averageValue;
}

int& Array::operator[](int index) {
    if (index >= size || index < 0) {
        throw std::out_of_range("Index " + std::to_string(index) +
                                " out of range for array of size "
                                + std::to_string(size));
    }

    return array[index];
}
