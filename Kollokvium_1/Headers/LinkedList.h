#ifndef KOLLOKVIUM_1_LINKEDLIST_H
#define KOLLOKVIUM_1_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void print();
    void insert(int value);

private:
    Node* head;

    void printRecursive(Node* current);
};

#endif //KOLLOKVIUM_1_LINKEDLIST_H
