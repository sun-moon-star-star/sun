/**
 * @file skiplist_test.cc
 * @author sun-moon-star-star
 */

#include "skiplist.h"

#include <string>

#include <gtest/gtest.h>

class SkipListTest : public testing::Test {
protected:
    void SetUp() override {
        sun::data_structure::skiplist_space::init_random_engine_with_time();
    }  
};

TEST_F(SkipListTest, tese_skiplist) {
    sun::data_structure::skiplist<int, std::string> list;
    ASSERT_EQ(list.size(), 0);

    std::pair<int, std::string> sets[]= {
        {1,"one"},
        {2, "two"},
        {10, "ten"},
        {5, "five"}
    };
    
    // insert nodes
    for (int i = 0; i < 4; ++i) {
        list.insert(sets[i].first, sets[i].second);
    }
    
    // check list size
    ASSERT_EQ(list.size(), 4);
    
    // skiplist is a sorted set
    sort(sets, sets + 4);
    
    // check for_each
    unsigned check_index = 0;
    auto check_func = [&check_index, &sets](int* key, std::string* value) {
        ASSERT_EQ(*key, sets[check_index].first);
        ASSERT_EQ(*value, sets[check_index].second);
        ++check_index;
    };
    list.for_each(check_func);
    ASSERT_EQ(check_index, list.size());
    
    // check for_each reverse
    check_index = list.size() - 1;
    auto check_func_reverse = [&check_index, &sets](int* key, std::string* value) {
        ASSERT_EQ(*key, sets[check_index].first);
        ASSERT_EQ(*value, sets[check_index].second);
        --check_index;
    };
    list.for_each(check_func_reverse, true);
    ASSERT_EQ(check_index, -1);
    
    std::string res = "default";
    // search if key not exists
    ASSERT_FALSE(list.search(-1, &res));
    ASSERT_EQ(res, "default");
    
    // search if key exist
    ASSERT_TRUE(list.search(5, &res));
    ASSERT_EQ(res, "five");
    
    // update key
    list.insert(1,"yi");
    
    // check if udpdate key
    ASSERT_TRUE(list.search(1, &res));
    ASSERT_EQ(res, "yi");
    
    // check swap
    sun::data_structure::skiplist<int, std::string> list2;
    list2.insert(3, "three");
    list2.insert(4, "four");
    list2.insert(6, "six");
    list.swap(&list2);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list2.size(), 4);
    
    // check erase
    ASSERT_TRUE(list.erase(6, &res));
    ASSERT_EQ(res, "six");
    ASSERT_FALSE(list.erase(6, &res));
    ASSERT_EQ(list.size(), 2);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}