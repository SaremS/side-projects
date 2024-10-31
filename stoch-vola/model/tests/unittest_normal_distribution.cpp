#include <vector>
#include <cmath>

#include "gtest/gtest.h"
#include "statistics/normal_distribution.h"

TEST(NormalDistribution, DefaultConstructor) {
  NormalDistribution nd;
  EXPECT_EQ(nd.getMean(), 0.0);
  EXPECT_EQ(nd.getStdDev(), 1.0);
}

TEST(NormalDistribution, Pdf) {
  NormalDistribution nd;
  EXPECT_NEAR(nd.pdf(0.0), 0.398942, 1e-5);
}

TEST(NormalDistribution, logPdf) {
  NormalDistribution nd;
  EXPECT_NEAR(nd.logLikelihood(0.0), std::log(0.398942), 1e-5);
}

TEST(NormalDistribution, sample) {
  NormalDistribution nd;
  std::vector<double> sample1 = nd.sample(10, 123);
  std::vector<double> sample2 = nd.sample(10, 123);

  for (int i=0; i<10; i++) {
    double s1 = sample1[i];
    double s2 = sample2[i];
    EXPECT_EQ(s1, s2);
  }
}
