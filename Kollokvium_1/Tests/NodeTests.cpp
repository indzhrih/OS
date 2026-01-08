#include <gtest/gtest.h>
#include "../Headers/Node.h"

TEST(NodeTest, ConstructorWithValue_SetsDataCorrectly) {
    Node node(42);
    EXPECT_EQ(node.data, 42);
}

TEST(NodeTest, ConstructorWithValue_SetsNextToNullptr) {
    Node node(42);
    EXPECT_EQ(node.next, nullptr);
}

TEST(NodeTest, CanSetNextPointer) {
    Node node1(1);
    Node node2(2);
    node1.next = &node2;
    EXPECT_EQ(node1.next->data, 2);
}
