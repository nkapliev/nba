#ifndef NBA_CONTAINER_STACK_H
#define NBA_CONTAINER_STACK_H

#include <stdlib.h>
#include <atomic>

// would be good to be able to do
// #include "Locks.hpp"
#include "../lock/Locks.hpp"

namespace nba
{

template <class T>
class NBStack
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef int64_t size_type;
    typedef const T& const_reference;

    // would be helpfull to use this in outer code
    typedef lock::SpinLock lock_type; 

private:
    struct Node
    {
        T*    data;
        Node* next;
    };

    std::atomic<size_type> m_size;
    std::atomic<Node*>     m_list;

public:
    NBStack()
    {
        m_size.store(0, std::memory_order_relaxed);
        m_list.store(nullptr, std::memory_order_relaxed);
    }
    
    // NBStack is not copy/move constructible
    NBStack(const NBStack& s) = delete;
    NBStack(NBStack&& s) = delete;

    // NBStack is no move/copy assignable
    NBStack& operator=(const NBStack& s) = delete;
    NBStack& operator=(NBStack&& s) = delete;

    ~NBStack()
    {
        while(!empty())
        {
            pop();
        }
    }

    bool empty() const
    {
        return m_size.load(std::memory_order_relaxed) == 0;
    }

    size_type size() const
    {
        return m_size.load(std::memory_order_relaxed);
    }

    const_reference top() const
    {
        if (!empty())
        {
            return *(m_list.load()->data);
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
        do
        {
            new_node->next = m_list;
        }
        while (m_list.compare_exchange_weak(new_node->next, new_node));
        m_size++;
    }

    void pop()
    {
        // it's not the right way
        // should be replaced with lock-free code
        if (!empty())
        {
            Node *top = m_list;
            m_list = m_list->next;
            delete top;
        }
    }

    void swap(stack& x)
    {
        // should be replaced with lock-free code
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
