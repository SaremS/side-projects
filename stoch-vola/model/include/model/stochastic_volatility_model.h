#ifndef STOCHASTIC_VOLATILITY_MODEL_H
#define STOCHASTIC_VOLATILITY_MODEL_H

#include <vector>
#include <cmath>

#include <Eigen/Dense>

#include "statistics/normal_distribution.h"
#include "statistics/particles.h"


class StochasticVolatilityModel {
  //As in 
  //Hautsch, Ou - Discrete-Time Stochastic Volatility Models and MCMC-Based Statistical Inference (2008)
  //Formulas 8a, 8b
  //https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=6d6e72a16927a813129a2b731a98b9ba4aec0f02
  private:
    double mu_;
    double phi_;
    double sigma_;

  public:
    StochasticVolatilityModel(double mu, double phi, double sigma);
    Particles particleFilter(const Eigen::VectorXd& y, const unsigned int& M, const unsigned int& seed = 123);
    double logLikelihood(const Eigen::VectorXd& y, const unsigned int& M, const unsigned int& seed = 123);
};

#endif
