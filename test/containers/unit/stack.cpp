#include <string.h>
#include <gtest/gtest.h>
#include <nba/containers/stack.hpp>

TEST(StackUnitTest, Empty) {
    nba::Stack<int> stack;

    EXPECT_TRUE(stack.empty());
    stack.push(1);
    EXPECT_FALSE(stack.empty());
}

TEST(StackUnitTest, PushPopSize) {
    nba::Stack<int> stack;

    EXPECT_EQ(stack.size(), 0);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
    stack.push(1);
    EXPECT_EQ(stack.size(), 1);
    stack.push(1);
    EXPECT_EQ(stack.size(), 2);
    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
}

TEST(StackUnitTest, Top) {
    nba::Stack<int> stack;

    try {
        stack.top();
    } catch (const std::logic_error& e) {
        EXPECT_EQ(strcmp(e.what(), "Calling `top` from empty stack is forbidden."), 0);
    }

    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    stack.pop();
    EXPECT_EQ(stack.top(), 1);
}

TEST(StackUnitTest, Swap) {
    nba::Stack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    stack1.push(3);

    nba::Stack<int> stack2;
    stack2.push(10);
    stack2.push(20);

    stack1.swap(stack2);

    EXPECT_EQ(stack1.size(), 2);
    EXPECT_EQ(stack1.top(), 20);
    stack1.pop();
    EXPECT_EQ(stack1.top(), 10);

    EXPECT_EQ(stack2.size(), 3);
    EXPECT_EQ(stack2.top(), 3);
    stack2.pop();
    EXPECT_EQ(stack2.top(), 2);
    stack2.pop();
    EXPECT_EQ(stack2.top(), 1);
}
