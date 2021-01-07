/**
 * @file heap_table_test.cc
 * @author sun-moon-star-star
 */

#include "sun/data_structure/hash_table.h"

#include <string>

#include <gtest/gtest.h>

class HashTableTest : public testing::Test {};

using sun::data_structure::hash_table;

TEST_F(HashTableTest, test_base) {
  hash_table<int, std::string> hash_table;
  hash_table.set(1, "one");
  hash_table.set(2, "two");
  hash_table.set(3, "three");

  ASSERT_EQ(hash_table.count(), 3);

  std::string res;
  ASSERT_TRUE(hash_table.get(1, &res));
  ASSERT_EQ(res, "one");

  ASSERT_TRUE(hash_table.get(2, &res));
  ASSERT_EQ(res, "two");

  ASSERT_TRUE(hash_table.erase(1, &res));
  ASSERT_EQ(res, "one");

  ASSERT_FALSE(hash_table.get(1, &res));

  auto it = hash_table.get_iterator();
  ASSERT_TRUE(it.valid());

  ASSERT_TRUE(it->key == 2 && it->value == "two" ||
              it->key == 3 && it->value == "three");
  ++it;
  ASSERT_TRUE(it.valid());
  ASSERT_TRUE(it->key == 2 && it->value == "two" ||
              it->key == 3 && it->value == "three");
  ++it;
  ASSERT_FALSE(it.valid());
}
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}