#include "../Headers/LinkedList.h"
#include <iostream>

LinkedList::LinkedList() : head(nullptr) {}

LinkedList::~LinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void LinkedList::printRecursive(Node* current) {
    if (current == nullptr) return;
    
    std::cout << current->data;
    if (current->next != nullptr) std::cout << " -> ";
    printRecursive(current->next);
}

void LinkedList::print() {
    std::cout << "Linked list: ";
    printRecursive(head);
    std::cout << std::endl;
}

void LinkedList::insert(int value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
}
