#include <vector>
#include <cmath>

#include "gtest/gtest.h"
#include "statistics/normal_distribution.h"

TEST(StochasticVolatility_NormalDistribution, DefaultConstructor) {
  NormalDistribution nd;
  EXPECT_EQ(nd.getMean(), 0.0);
  EXPECT_EQ(nd.getStdDev(), 1.0);
}

TEST(StochasticVolatility_NormalDistribution, Pdf) {
  NormalDistribution nd;
  EXPECT_NEAR(nd.pdf(0.0), 0.398942, 1e-5);
}

TEST(StochasticVolatility_NormalDistribution, logPdf) {
  NormalDistribution nd;
  EXPECT_NEAR(nd.logLikelihood(0.0), std::log(0.398942), 1e-5);
}

TEST(StochasticVolatility_NormalDistribution, sample) {
  NormalDistribution nd;
  std::vector<double> sample1 = nd.sample(10, 123);
  std::vector<double> sample2 = nd.sample(10, 123);

  for (int i=0; i<10; i++) {
    double s1 = sample1[i];
    double s2 = sample2[i];
    EXPECT_EQ(s1, s2);
  }
}

TEST(StochasticVolatility_IndependentVectorNormal, DefaultConstructor) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  EXPECT_EQ(ivn.getMeans(), means);
  EXPECT_EQ(ivn.getStdDevs(), stdDevs);
}

TEST(StochasticVolatility_IndependentVectorNormal, Pdf) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Eigen::VectorXd x(3);
  x << 1.0, 2.0, 3.0;
  EXPECT_NEAR(ivn.pdfs(x)(0), 3.989422, 1e-5);
}

TEST(StochasticVolatility_IndependentVectorNormal, PdfSingle) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Eigen::VectorXd x(3);
  x << 1.0, 2.0, 3.0;
  EXPECT_NEAR(ivn.pdfs(x)(0), ivn.pdfs(1.0)(0), 1e-5);
}

TEST(StochasticVolatility_IndependentVectorNormal, LogLikelihoods) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Eigen::VectorXd x(3);
  x << 1.0, 2.0, 3.0;
  EXPECT_NEAR(ivn.logLikelihoods(x)(0), 1.38364655, 1e-5);
}

TEST(StochasticVolatility_IndependentVectorNormal, LogLikelihoodsSingle) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Eigen::VectorXd x(3);
  x << 1.0, 2.0, 3.0;
  EXPECT_NEAR(ivn.logLikelihoods(x)(0), ivn.logLikelihoods(1.0)(0), 1e-5);
}

TEST(StochasticVolatility_IndependentVectorNormal, ColWiseLogLikelihoods) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Eigen::MatrixXd x(3, 3);
  x << 1.0, 2.0, 3.0,
       1.0, 2.0, 3.0,
       1.0, 2.0, 3.0;
  Eigen::MatrixXd logLikelihoods = ivn.colWiseLogLikelihoods(x);
  EXPECT_NEAR(logLikelihoods(0,0), 1.38364655, 1e-5);

}

TEST(StochasticVolatility_IndependentVectorNormal, sample) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Eigen::VectorXd sample1 = ivn.sample(123);
  Eigen::VectorXd sample2 = ivn.sample(123);
  Eigen::VectorXd sample3 = ivn.sample(124);
  for (int i=0; i<3; i++) {
    double s1 = sample1(i);
    double s2 = sample2(i);
    double s3 = sample3(i);
    EXPECT_EQ(s1, s2);
    EXPECT_NE(s1, s3);
  }
}
