#ifndef NBA_CONTAINER_NBSTACK_H
#define NBA_CONTAINER_NBSTACK_H

#include <stdlib.h>
#include <atomic>

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

    // Assume that val has copy constructor
    void push(const value_type& val)
    {
        Node *new_node = new Node(val);
        do
        {
            new_node->next.store(m_list.load(std::memory_order_acquire), std::memory_order_relaxed);
        }
        while (m_list.compare_exchange_weak(new_node->next, new_node, std::memory_order_release));
    }

    void pop()
    {
        // it's not the right way
        // should be replaced with lock-free code
    }

    void swap(NBStack& x)
    {
        // should be replaced with lock-free code
    };

    //emplace TODO DO we need it?
};

template <typename T>
class NBStack<T>::Node
{
    Node(const NBStack::value_type& data)
    {
        m_data = new NBStack::value_type(data);
        m_next.store(nullptr, std::memory_order_relaxed);
    }

    const NBStack::value_type& get() const
    {
        return *m_data;
    }

    NBStack::value_type& get()
    {
        return *m_data;
    }
      
    std::atomic<Node*> & next()
    {
        return m_next.load(std::memory_order_relaxed);
    }

    const std::atomic<Node*> & next() const
    {
        return m_next.load(std::memory_order_relaxed);
    }
    
    NBStack::size_type size() const
    {
        if (m_next.load(std::memory_order_relaxed))
            return m_next.load(std::memory_order_relaxed)->size() + 1;
        return 1;
    }

    ~Node()
    {
        delete m_data;
    }

private:
    NBStack::value_type*  m_data;
    std::atomic<Node*>    m_next;
};

} // namespace nba

#endif // NBA_CONTAINER_NBSTACK_H
