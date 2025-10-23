#include <iostream>
#include <limits>
#include "../Headers/Menu.h"
#include "../Headers/Factorial.h"
#include "../Headers/DuplicateRemover.h"
#include "../Headers/LinkedList.h"

void Menu::printMenuVariants() {
    std::cout << "Choose task: \n" <<
             "1) Factorial\n" <<
             "2) Container without duplicates\n" <<
             "3) Linked list\n" <<
             "0) Exit" << std::endl;
}

void Menu::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int Menu::getIntInput() {
    int value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Invalid input! Please enter an integer: ";
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            return value;
        }
    }
}

int Menu::getNonNegativeIntInput() {
    int value;
    while (true) {
        value = getIntInput();
        if (value >= 0) return value;
        else std::cout << "Value must be non-negative! Try again: ";
    }
}

int Menu::getPositiveIntInput() {
    int value;
    while (true) {
        value = getIntInput();
        if (value > 0) return value;
        else std::cout << "Value must be positive! Try again: ";
    }
}

void Menu::handleUserChoice() {
    while (true) {
        printMenuVariants();
        std::cout << "Enter your choice: ";
        userChoice = getIntInput();

        switch (userChoice) {
            case 1:
                factorialTask();
                break;
            case 2:
                duplicateTask();
                break;
            case 3:
                linkedListTask();
                break;
            case 0:
                exit();
                return;
            default:
                wrongInput();
                break;
        }
    }
}

void Menu::factorialTask() {
    std::cout << "Input n: ";
    int n = getNonNegativeIntInput();

    Factorial factorial = Factorial(n);
    std::cout << "Factorials: ";
    factorial.printFactorials();
}

void Menu::duplicateTask() {
    std::cout << "Input n: ";
    int n = getPositiveIntInput();

    std::vector<int> vect(n);
    std::cout << "Enter array elements: " << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "Element " << (i + 1) << ": ";
        vect[i] = getIntInput();
    }

    DuplicateRemover duplicateRemover;
    duplicateRemover.removeDuplicates(vect);
    std::cout << "Without duplicates: ";
    for (int i : vect) std::cout << i << " ";
    std::cout << std::endl;
}

void Menu::linkedListTask() {
    LinkedList list;

    std::cout << "Input number of elements: ";
    int n = getNonNegativeIntInput();

    if (n > 0) {
        std::cout << "Enter elements: " << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << "Element " << (i + 1) << ": ";
            int value = getIntInput();
            list.insert(value);
        }
    }

    std::cout << "Linked list: ";
    list.print();
}

void Menu::exit() {
    std::cout << "Goodbye!" << std::endl;
}

void Menu::wrongInput() {
    std::cout << "Wrong input, try again" << std::endl;
}

void Menu::openMenu() {
    handleUserChoice();
}
