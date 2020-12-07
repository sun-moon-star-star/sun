/**
 * @file heap_test.cc
 * @author sun-moon-star-star
 */

#include "heap.h"

#include <string>

#include <gtest/gtest.h>

class HeapTest : public testing::Test {};

TEST_F(HeapTest, test_int) {
    int a[] = {1, 4, -6, 8, 2, 3, 5, 0, -7, -3, 9};
    unsigned size = sizeof(a) / sizeof(int);

    sun::data_structure::heap<int> heap;
    heap.init(&a[0], size);
    ASSERT_EQ(heap.size(), size);
    std::sort(a, a + size, sun::data_structure::greater<int>());

    int out;
    for (int i = 0; i < size; ++i) {
        heap.pop(&out);
        ASSERT_EQ(out, a[i]);
    }
    ASSERT_EQ(heap.size(), 0);
}

TEST_F(HeapTest, test_class) {
    struct test {
        test() {}
        test(int key, std::string value): key(key), value(value){}
        bool operator<(const test& p) const {
            return key > p.key;
        }
        int key;
        std::string value;
    };

    sun::data_structure::heap<test> heap;
    heap.push(1, "one");
    heap.push(5, "five");
    heap.push(test(3, "three"));
    heap.push(test(0, "zero"));

    ASSERT_EQ(heap.size(), 4);
    
    test out;
    heap.pop(&out);
    ASSERT_EQ(out.key, 0);
    ASSERT_EQ(out.value, "zero");

    heap.pop(&out);
    ASSERT_EQ(out.key, 1);
    ASSERT_EQ(out.value, "one");

    heap.pop(&out);
    ASSERT_EQ(out.key, 3);
    ASSERT_EQ(out.value, "three");

    heap.pop(&out);
    ASSERT_EQ(out.key, 5);
    ASSERT_EQ(out.value, "five");

    ASSERT_EQ(heap.size(), 0);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}