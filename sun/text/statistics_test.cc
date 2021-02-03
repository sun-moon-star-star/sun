/**
 * @file statistics_test.cc
 * @author sun-moon-star-star
 */

#include "sun/text/statistics.h"

#include "sun/util/common.h"

#include <string>

#include <gtest/gtest.h>

class StatisticsTest : public testing::Test {};

TEST_F(StatisticsTest, test_base) {
  sun::text::filter_ptr filter = sun::text::filter::make_filter("zhao");
  sun::text::filter_ptr filter_2 =
      sun::text::filter::make_filter("starstar", filter);

  ASSERT_TRUE(filter->match("zhaolu"));
  ASSERT_FALSE(filter->match("starstarzzz"));

  ASSERT_TRUE(filter_2->match("zhaolustarstar"));
  ASSERT_FALSE(filter_2->match("zhaolu"));
  ASSERT_FALSE(filter_2->match("starstarzzz"));
}

TEST_MAIN