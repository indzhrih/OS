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

    int& operator[](int index);
private:
    int size;
    int* array = nullptr;
};

#endif
