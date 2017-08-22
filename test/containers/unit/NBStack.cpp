#include <gtest/gtest.h>
#include <nba/containers/NBStack.hpp>

TEST(NBStackUnitTest, IsEmpty) {
    nba::NBStack<int> stack;
    EXPECT_TRUE(stack.empty());
}
