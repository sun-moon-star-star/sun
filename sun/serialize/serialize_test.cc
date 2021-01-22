/**
 * @file serialize_test.cc
 * @author sun-moon-star-star
 */

#include "sun/serialize/serialize.h"

#include "sun/util/common.h"

#include <string>

#include <gtest/gtest.h>

class SerializeTest : public testing::Test {};

TEST_F(SerializeTest, test_base) {
  char chs[5] = "zhao";
  std::string data = ::sun::serialize<decltype(chs)>(chs);

  char chs2[5] = {0};
  ::sun::unserialize<decltype(chs2)>(data, &chs2);

  ASSERT_EQ(strcmp(chs, chs2), 0);
}

TEST_MAIN