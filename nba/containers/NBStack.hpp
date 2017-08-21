#ifndef NBA_CONTAINER_NBSTACK_H
#define NBA_CONTAINER_NBSTACK_H

#include <stdlib.h>
#include <atomic>

// would be good to be able to do
// #include "Locks.hpp"
// I removed relative paths by changing makefile. Looks good now?
#include <nba/lock/Lock.hpp>

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
    class Node;

    std::atomic<Node*>     m_list;

public:
    NBStack()
    {
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
        return m_list.load(std::memory_order_relaxed) == nullptr;
    }

    size_type size() const
    {
        if (m_list.load(std::memory_order_relaxed) == nullptr)
            return 0;
        return m_list.load(std::memory_order_relaxed)->size();
    }

    const_reference top() const
    {
        if (!empty())
        {
            return m_list.load(std::memory_order_relaxed).get();
        }
        else
        {
            NBStack::value_type val;
            return val;
        }
    }

    // Assume that val has copy constructor
    void push(const value_type& val)
    {
        Node *new_node = new Node(val);
        do
        {
            new_node->next = m_list.load(std::memory_order_acquire);
        }
        while (m_list.compare_exchange_weak(new_node->next, new_node, std::memory_order_release));
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
        m_list = x.m_list;
        x.m_list = m_list;
    };

    //emplace TODO DO we need it?
};

class NBStack::Node
{
    Node(const NBStack::value_type& data)
    {
        m_data = new NBStack::value_type(data);
        m_next = nullptr;
    }

    const NBStack::value_type& get() const
    {
        return *m_data;
    }

    NBStack::value_type& get()
    {
        return *m_data;
    }
      
    Node*& next()
    {
        return m_next;
    }

    const Node*& next() const
    {
        return m_next;
    }
    
    NBStack::size_type size() const
    {
        if (m_next) return m_next.size() + 1;
        return 1;
    }

    ~Node()
    {
        delete m_data;
    }

private:
    NBStack::value_type*  m_data;
    Node*                 m_next;
};

} // namespace nba

#endif // NBA_CONTAINER_NBSTACK_H
