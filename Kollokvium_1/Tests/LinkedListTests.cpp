#include <gtest/gtest.h>
#include "../Headers/LinkedList.h"

TEST(LinkedListTest, DefaultConstructor_CreatesEmptyList) {
    LinkedList list;
    SUCCEED();
}

TEST(LinkedListTest, PrintEmptyList_OutputsEmptyMessage) {
    LinkedList list;
    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Linked list: \n");
}

TEST(LinkedListTest, InsertSingleElement_PrintsCorrectly) {
    LinkedList list;
    list.insert(42);
    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Linked list: 42\n");
}

TEST(LinkedListTest, InsertMultipleElements_PrintsInReverseOrder) {
    LinkedList list;
    list.insert(3);
    list.insert(2);
    list.insert(1);
    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Linked list: 1 -> 2 -> 3\n");
}

TEST(LinkedListTest, Destructor_DoesNotCrash) {
    LinkedList* list = new LinkedList();
    list->insert(1);
    list->insert(2);
    delete list;
    SUCCEED();
}
