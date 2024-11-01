#include <vector>
#include <cmath>

#include "gtest/gtest.h"

#include "model/stochastic_volatility_model.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"

TEST(StochasticVolatilityModel, ParticleFilter) {
  std::vector<double> y = {1.0, 2.0, 3.0};
  StochasticVolatilityModel svm(0.0, 0.0, 0.0);
  Particles<double> p = svm.particleFilter(y, 10, 123);
  EXPECT_EQ(p.getParticleCount(), 10);
  EXPECT_EQ(p.getParticleLength(), 3);
}

TEST(StochasticVolatilityModel, LogLikelihood) {
  std::vector<double> y = {1.0, 2.0, 3.0};
  StochasticVolatilityModel svm(0.0, 0.0, 0.0);
  double logLikelihood = svm.logLikelihood(y, 10, 123);
  EXPECT_TRUE(std::isfinite(logLikelihood));
}
