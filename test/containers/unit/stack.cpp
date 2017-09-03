#include <gtest/gtest.h>
#include <nba/containers/stack.hpp>

TEST(StackUnitTest, Empty) {
    nba::Stack<int> stack;

    EXPECT_TRUE(stack.empty());
    stack.push(1);
    EXPECT_FALSE(stack.empty());
}

TEST(StackUnitTest, Size) {
    nba::Stack<int> stack;

    EXPECT_EQ(stack.size(), 0);
    stack.push(1);
    EXPECT_EQ(stack.size(), 1);
    stack.push(1);
    EXPECT_EQ(stack.size(), 2);
    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
}
