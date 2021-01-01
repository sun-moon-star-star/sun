/**
 * @file heap_table_test.cc
 * @author sun-moon-star-star
 */

#include "sun/data_structure/hash_table.h"

#include <string>

#include <gtest/gtest.h>

class HashTableTest : public testing::Test {};

TEST_F(HashTableTest, test_hash_table) {
  sun::data_structure::hash_table<int, std::string> hash_table;
  hash_table.set(1, "one");
  hash_table.set(2, "two");

  ASSERT_EQ(hash_table.count(), 2);

  std::string res;
  ASSERT_TRUE(hash_table.get(1, &res));
  ASSERT_EQ(res, "one");

  ASSERT_TRUE(hash_table.get(2, &res));
  ASSERT_EQ(res, "two");

  ASSERT_TRUE(hash_table.erase(1, &res));
  ASSERT_EQ(res, "one");

  ASSERT_FALSE(hash_table.get(1, &res));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}