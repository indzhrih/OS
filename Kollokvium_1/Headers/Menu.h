#ifndef KOLLOKVIUM_1_MENU_H
#define KOLLOKVIUM_1_MENU_H
class Menu {
public:
    void openMenu();
private:
    void printMenuVariants();
    void handleUserChoice();
    void factorialTask();
    void duplicateTask();
    void linkedListTask();
    void exit();
    void wrongInput();

    int getIntInput();
    int getNonNegativeIntInput();
    int getPositiveIntInput();
    void clearInputBuffer();

    int userChoice;
};

#endif //KOLLOKVIUM_1_MENU_H
