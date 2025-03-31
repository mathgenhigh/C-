#include <iostream>

struct Node {
    int data;
    Node* next;
    Node* prev;
};

Node* createCircularList(int n) {
    if (n < 1) return nullptr;

    Node* head = new Node;
    head->data = 1;
    head->next = head;
    head->prev = head;
    Node* current = head;

    for (int i = 2; i <= n; ++i) {
        Node* newNode = new Node;
        newNode->data = i;
        newNode->next = head;
        newNode->prev = current;
        current->next = newNode;
        head->prev = newNode;
        current = newNode;
    }

    return head;
}

int josephus(int n, int k) {
    if (n < 1 || k < 1) {
        std::cout << "Invalid input. n and k should be positive integers." << std::endl;
        return -1;
    }

    Node* head = createCircularList(n);
    Node* current = head;

    while (current->next != current) {
        for (int i = 1; i < k; ++i) {
            current = current->next;
        }

        Node* toDelete = current;
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
        current = toDelete->next;
        delete toDelete;
    }

    int survivor = current->data;
    delete current;
    return survivor;
}

int main() {
    int n, k;
    std::cout << "Enter the number of people (n): ";
    std::cin >> n;
    std::cout << "Enter the number of step size (k): ";
    std::cin >> k;

    int survivor = josephus(n, k);
    std::cout << "The survivor is: " << survivor << std::endl;

    return 0;
}

