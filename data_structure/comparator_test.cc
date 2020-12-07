/**
 * @file comparator_test.cc
 * @author sun-moon-star-star
 */

#include "comparator.h"

#include <gtest/gtest.h>

class ComparatorTest : public testing::Test {};

TEST_F(ComparatorTest, test_int) {
    int x = 1, y = 2;
    ASSERT_TRUE(sun::data_structure::less<int>()(x,y));
    ASSERT_FALSE(sun::data_structure::greater<int>()(x,y));
}

TEST_F(ComparatorTest, test_double) {
    double x = 1.1111, y = 1.11111;
    ASSERT_TRUE(sun::data_structure::less<double>()(x,y));
    ASSERT_FALSE(sun::data_structure::greater<double>()(x,y));
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
