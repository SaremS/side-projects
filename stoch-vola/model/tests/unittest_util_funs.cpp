#include <vector>

#include "gtest/gtest.h"
#include "statistics/util_funs.h"

TEST(utilfunsTest, Mean) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_DOUBLE_EQ(utilfuns::mean(data), 3.0);

    data = {2.5, 3.5, 4.5, 5.5};
    EXPECT_DOUBLE_EQ(utilfuns::mean(data), 4.0);

    data = {10.0};
    EXPECT_DOUBLE_EQ(utilfuns::mean(data), 10.0);
}

TEST(utilfunsTest, StandardDeviation) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_NEAR(utilfuns::standardDeviation(data), 1.414213, 1e-5);

    data = {2.5, 3.5, 4.5, 5.5};
    EXPECT_NEAR(utilfuns::standardDeviation(data), 1.11803, 1e-5);

    data = {10.0, 10.0, 10.0, 10.0};
    EXPECT_NEAR(utilfuns::standardDeviation(data), 0.0, 1e-5);
}

TEST(utilfunsTest, Quantile) {
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 0.25), 2.0);
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 0.5), 3.0);
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 0.75), 4.0);
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 1.0), 5.0);

    data = {10.0, 20.0, 30.0, 40.0};
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 0.0), 10.0);
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 0.5), 25.0);
    EXPECT_DOUBLE_EQ(utilfuns::quantile(data, 1.0), 40.0);
}
