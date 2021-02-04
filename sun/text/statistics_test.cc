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
  sun::text::filter_ptr filter = sun::text::base_filter::make_filter("zhao");
  sun::text::filter_ptr filter_2 =
      sun::text::base_filter::and_filter("starstar", filter);

  ASSERT_TRUE(filter->match("zhaolu"));
  ASSERT_FALSE(filter->match("starstarzzz"));

  ASSERT_TRUE(filter_2->match("zhaolustarstar"));
  ASSERT_FALSE(filter_2->match("zhaolu"));
  ASSERT_FALSE(filter_2->match("starstarzzz"));

  sun::text::filter_ptr filter_3 =
      sun::text::base_filter::or_filter("starstar", filter);

  ASSERT_TRUE(filter_3->match("zhaolustarstar"));
  ASSERT_TRUE(filter_3->match("zhaolu"));
  ASSERT_TRUE(filter_3->match("starstarzzz"));
}

TEST_MAIN