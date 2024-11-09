#include <vector>
#include <cmath>

#include "gtest/gtest.h"

#include "model/stochastic_volatility_model.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"

TEST(StochasticVolatility_StochasticVolatilityModel, ParticleFilter) {
  Eigen::VectorXd y(3);
  y << 1.0 , 2.0, 3.0;

  StochasticVolatilityModel svm(0.0, 0.0, 0.0);
  Particles p = svm.particleFilter(y, 10, 123);
  EXPECT_EQ(p.getParticleCount(), 10);
  EXPECT_EQ(p.getParticleLength(), 3);
}

TEST(StochasticVolatility_StochasticVolatilityModel, LogLikelihood) {
  Eigen::VectorXd y(3);
  y << 1.0 , 2.0, 3.0;

  StochasticVolatilityModel svm(0.0, 0.0, 0.0);
  double logLikelihood = svm.logLikelihood(y, 10, 123);
  EXPECT_TRUE(std::isfinite(logLikelihood));
}
