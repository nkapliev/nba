#include <gtest/gtest.h>
#include <nba/containers/stack.hpp>

TEST(StackUnitTest, IsEmpty) {
    nba::Stack<int> stack;
    EXPECT_TRUE(stack.empty());
}
