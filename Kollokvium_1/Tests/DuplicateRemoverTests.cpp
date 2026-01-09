#include <gtest/gtest.h>
#include "../Headers/DuplicateRemover.h"

TEST(DuplicateRemoverTest, RemoveDuplicatesFromEmptyVector_KeepsVectorEmpty) {
    DuplicateRemover remover;
    std::vector<int> vect;
    remover.removeDuplicates(vect);
    EXPECT_TRUE(vect.empty());
}

TEST(DuplicateRemoverTest, RemoveDuplicatesFromUniqueVector_PreservesAllElements) {
    DuplicateRemover remover;
    std::vector<int> vect = {1, 2, 3, 4, 5};
    remover.removeDuplicates(vect);
    EXPECT_EQ(vect.size(), 5);
}

TEST(DuplicateRemoverTest, RemoveDuplicatesFromAllSameElements_LeavesOneElement) {
    DuplicateRemover remover;
    std::vector<int> vect = {1, 1, 1, 1, 1};
    remover.removeDuplicates(vect);
    EXPECT_EQ(vect.size(), 1);
}

TEST(DuplicateRemoverTest, RemoveDuplicatesFromMixedVector_ReturnsSortedUniqueElements) {
    DuplicateRemover remover;
    std::vector<int> vect = {3, 1, 2, 2, 3, 4};
    remover.removeDuplicates(vect);
    std::vector<int> expected = {1, 2, 3, 4};
    EXPECT_EQ(vect, expected);
}

TEST(DuplicateRemoverTest, RemoveDuplicatesWithNegativeNumbers_WorksCorrectly) {
    DuplicateRemover remover;
    std::vector<int> vect = {-1, -2, -1, 0, -2};
    remover.removeDuplicates(vect);
    std::vector<int> expected = {-2, -1, 0};
    EXPECT_EQ(vect, expected);
}
