#include <vector>
#include <cmath>
#include <stdexcept>

#include "model/stochastic_volatility_model.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"


StochasticVolatilityModel::StochasticVolatilityModel(double phi, double sigma, double beta) : phi_(phi), sigma_(sigma), beta_(beta) {}

