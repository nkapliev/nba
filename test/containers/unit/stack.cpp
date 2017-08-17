#include <nba/containers/stack.hpp>
#include "stdio.h"

int main() {
    Stack<int> s;

    s.push(1);
    s.push(2);
    s.push(3);

    printf("%d\n", s.top());

    s.pop();
    printf("%d\n", s.top());
}
