/**
 * @file common_test.cc
 * @author sun-moon-star-star
 */

#include "sun/util/common.h"

#include <string>

#include <gtest/gtest.h>

class CommonTest : public testing::Test {};

TEST_F(CommonTest, test_common) {
  auto func = [](int id) -> std::string {
    if (id == 12) return "12";
    if (id == 34) return "34";
    return "func";
  };

  auto func2 = [&func](int id) -> std::string {
    IF_TRUE_RETURN(id == 12, func(12));
    IF_TRUE_RETURN(id == 34, func(34));
    IF_TRUE_RETURN(id == 56, func(12));
    IF_TRUE_RETURN(id == 78, func(12) + func(34));
    IF_FALSE_RETURN(id == 0, func(id));
    return "func2";
  };

  ASSERT_EQ(func2(12), "12");
  ASSERT_EQ(func2(34), "34");
  ASSERT_EQ(func2(56), "12");
  ASSERT_EQ(func2(78), "1234");
  ASSERT_EQ(func2(-1), "func");
  ASSERT_EQ(func2(0), "func2");
}

TEST_MAIN