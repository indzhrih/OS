#ifndef ARRAY_H
#define ARRAY_H

class Array {
public:
    Array();
    Array(int size);
    ~Array();

    void inputArray();
    void printArray();
    void switchMaxAndMinOnAverage();

    void checkArrayInitialization();

    int findMaxElement();
    int findMinElement();
    int calculateAverageValue();

    int& operator[](int index);

private:
    int maxElementIndex = 0;
    int minElementIndex = 0;
    int averageValue = 0;

    int size;
    int* array = nullptr;
};

#endif
