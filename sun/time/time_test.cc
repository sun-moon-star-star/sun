/**
 * @file time_test.cc
 * @author sun-moon-star-star
 */

#include "sun/time/time.h"

#include <gtest/gtest.h>

class TimeTest : public testing::Test {};

TEST_F(TimeTest, test_time) {
  sun::time t;
  t.year = 2021;
  t.month = 1;
  t.day = 23;
  ASSERT_EQ(t.week(), 6);
  ASSERT_EQ(t.week_string(), "Sat");
}

TEST_MAIN
