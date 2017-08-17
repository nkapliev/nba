#ifndef NBA_CONTAINER_STACK_H
#define NBA_CONTAINER_STACK_H

#include <stdlib.h>

template <class T>
class Stack {
private:
    struct Node {
        T *data;
        Node *next;
    };

    size_t size;
    Node *list;

public:
    Stack() {
        size = 0;
        list = nullptr;
    }

    ~Stack() {
        while(!empty()) {
            pop();
        }
    }

    bool empty() const {
        return size == 0;
    }

    size_t size() const {
        return size;
    }

    const T& top() const {
        if (!empty()) {
            return list->data;
        } else {
            // TODO
        }
    }

    void push(T const & val) {
        Node *new_node = new Node;
        new_node->data = val;
        new_node->next = list;

        list = new_node;
        size++;
    }

    void pop() {
        if (!empty()) {
            Node *top = list;
            list = list->next;
            delete top;
        }
    }

    void swap(stack& x) {
        Node *tmp_list = list;
        size_t tmp_size = size;

        list = x.list;
        x.list = list;

        size = x.size;
        x.size = size;
    };

    //emplace TODO DO we need it?
};
#endif
