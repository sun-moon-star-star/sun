/**
 * @file common_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/json.h"

#include <string>

#include <gtest/gtest.h>

class JsonTest : public testing::Test {};

TEST_F(JsonTest, test_json) {
  std::string in = R"({"Age": 20, "Name": "zhao"})";
  std::string out = R"({"Age": 19, "Name": "colin"})";
  std::string result;

  int ret = sun::util::updateJsonField(in, out, {"Name"}, &result);
  ASSERT_EQ(ret, 0);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}