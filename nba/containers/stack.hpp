#ifndef NBA_CONTAINER_STACK_H
#define NBA_CONTAINER_STACK_H

#include <stdlib.h>

namespace nba
{

template <class T>
class Stack
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef int64_t size_type;
    typedef const T& const_reference;

private:
    struct Node
    {
        T*    data;
        Node* next;
    };

    size_type m_size;
    Node*     m_list;

public:
    Stack()
    {
        m_size = 0;
        m_list = nullptr;
    }
    
    // Stack is not copy/move constructible
    Stack(const Stack& s) = delete;
    Stack(Stack&& s) = delete;

    // Stack is no move/copy assignable
    Stack& operator=(const Stack& s) = delete;
    Stack& operator=(Stack&& s) = delete;

    ~Stack()
    {
        while(!empty())
        {
            pop();
        }
    }

    bool empty() const
    {
        return m_size == 0;
    }

    size_type size() const
    {
        return m_size;
    }

    const_reference top() const
    {
        if (!empty())
        {
            return *(m_list->data);
        } else
        {
            Node n;
            return n; 
        }
    }

    // Assume that val has copy constructor
    void push(const value_type& val)
    {
        Node *new_node = new Node();
        new_node->data = new value_type(val);

        new_node->next = m_list;
        m_list = new_node;
        m_size++;
    }

    void pop()
    {
        if (!empty())
        {
            Node *top = m_list;
            m_list = m_list->next;
            delete top;
        }
    }

    void swap(stack& x)
    {
        Node *tmp_list = m_list;
        size_t tmp_size = m_size;

        m_list = x.m_list;
        x.m_list = m_list;

        m_size = x.m_size;
        x.m_size = m_size;
    };

    //emplace TODO DO we need it?
};

} // namespace nba

#endif
