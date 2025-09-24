#include <iostream>
#include <string>
#include "Headers/Array.h"
#include "Headers/ThreadHandler.h"

int main() {
    ThreadHandler threadHandler;
    int n;

    try {
        while (true) {
            std::cout << "Input array size: ";
            if (std::cin >> n && n > 0) break;

            std::cerr << "Incorrect size, try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        Array array(n);
        array.inputArray();
        array.printArray();

        threadHandler.createThread(std::string("min_max"), array);
        threadHandler.createThread(std::string("average"), array);

        array.switchMaxAndMinOnAverage();
        array.printArray();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
