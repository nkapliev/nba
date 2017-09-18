#ifndef NBA_CONTAINER_NBCOUNTSTACK_H
#define NBA_CONTAINER_NBCOUNTSTACK_H

#include <atomic>

namespace nba {
template<typename T>
class NBCountStack {
private:
    struct node {
        node *next;
        std::shared_ptr <T> data;

        node(const T &d, node *n = 0)
                : next(n), data(std::make_shared<T>(d)) {}
    };

    std::atomic<node *> head;
    std::atomic<node *> to_be_deleted;
    std::atomic<unsigned> threads_in_pop;

public:
    void push(const T &data) {
        node *new_node = new node(data, head.load());
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr <T> pop() {
        ++threads_in_pop;
        node *old_head = head;
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next));
        std::shared_ptr <T> res;
        if (old_head)
            res.swap(old_head->data);

        try_reclaim(old_head);

        return res;
    }

    bool empty() const
    {
        return head.;
    }

private:
    void chain_node_list(node *first, node *last) {
        last->next = to_be_deleted;
        while (!to_be_deleted.compare_exchange_weak(last->next, first));
    }

    void chain_node_list(node *nodes) {
        node *last = nodes;
        while (node *const next = last->next)
            last = next;
        chain_pending_nodes(nodes, last);
    }

    void chain_one_node(node *n) {
        chain_pending_nodes(n, n);
    }

    static void delete_nodes(node *nodes) {
        while (nodes) {
            node *next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void try_reclaim(node *old_head) {
        if (threads_in_pop == 1) {
            node *nodes_to_delete = to_be_deleted.exchange(NULL);
            if (!--threads_in_pop) {
                delete_nodes(nodes_to_delete);
            } else if (nodes_to_delete) {
                chain_node_list(nodes_to_delete);
            }
            delete old_head;
        } else {
            chain_one_node(old_head);
            --threads_in_pop;
        }
    }
};
}
#endif // NBA_CONTAINER_NBCOUNTSTACK_H
