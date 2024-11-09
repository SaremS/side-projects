#include <vector>
#include <cmath>
#include <stdexcept>

#include <Eigen/Dense>

#include "pybind11/pybind11.h"
#include "pybind11/eigen.h"
#include "pybind11/stl.h"

#include "model/stochastic_volatility_model.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"


namespace py = pybind11;

PYBIND11_MODULE(stochastic_volatility_model,m) {
	py::class_<StochasticVolatilityModel>(m, "StochasticVolatilityModel")
		.def(py::init<double, double, double>(),
				py::arg("mu") = 0.0,
				py::arg("phi") = 0.0,
				py::arg("sigma") = 0.0)
		.def("particleFilter", &StochasticVolatilityModel::particleFilter)
    .def("logLikelihood", &StochasticVolatilityModel::logLikelihood);

	py::class_<Particles>(m, "Particles")
		.def("getParticles", &Particles::getParticlesAsEigenMatrix);
}


StochasticVolatilityModel::StochasticVolatilityModel(double mu, double phi, double sigma) : mu_(mu), phi_(phi), sigma_(sigma) {}

Particles StochasticVolatilityModel::particleFilter(const Eigen::VectorXd& y, const unsigned int& nParticles, const unsigned int& seed) {
  unsigned int T = y.size();

  double mu = mu_;
  double phi = std::tanh(phi_);
  double sigma = std::exp(sigma_);

  Particles particles = Particles(IndependentVectorNormal(mu, sigma, nParticles), T+1, seed);

  int loopSeed = seed; //unique seed for each loop iteration

  for (int t=1; t<=T; t++) {
    Eigen::VectorXd latestParticles = particles.getLatestParticles(); //particles at t-1

    latestParticles = mu + phi * (latestParticles.array() - mu) + IndependentVectorNormal(0.0, sigma, nParticles).sample(loopSeed).array();

    Eigen::VectorXd means = Eigen::VectorXd::Zero(nParticles);
    Eigen::VectorXd stds = (latestParticles / 2.0).array().exp();

    Eigen::VectorXd likelihoods = IndependentVectorNormal(means, stds).pdfs(y[t-1]);
    double lSum = likelihoods.sum();

    particles.appendParticles(latestParticles); //particles at t
    
    if (lSum > 0.0) {//avoid degenerate case
      Eigen::VectorXd weights = likelihoods/lSum;
      std::vector<double> weightsVec(weights.data(), weights.data() + nParticles);
      
      particles.resampleParticles(weightsVec, loopSeed);
    }
  }

  return particles.getParticlesWithoutInit();
}


double StochasticVolatilityModel::logLikelihood(const Eigen::VectorXd& y, const unsigned int& nParticles, const unsigned int& seed) {
  unsigned int T = y.size();

  double mu = mu_;
  double phi = std::tanh(phi_);
  double sigma = std::exp(sigma_);

  Particles particles = Particles(IndependentVectorNormal(mu, sigma, nParticles), T+1, seed);

  int loopSeed = seed; //unique seed for each loop iteration
  double logLikeSum = 0.0;

  for (int t=1; t<=T; t++) {
    Eigen::VectorXd latestParticles = particles.getLatestParticles(); //particles at t-1

    latestParticles = mu + phi * (latestParticles.array() - mu) + IndependentVectorNormal(0.0, sigma, nParticles).sample(loopSeed).array();

    Eigen::VectorXd means = Eigen::VectorXd::Zero(nParticles);
    Eigen::VectorXd stds = (latestParticles / 2.0).array().exp();

    Eigen::VectorXd logLikelihoods = IndependentVectorNormal(means, stds).logLikelihoods(y[t-1]);
    Eigen::VectorXd likelihoods = logLikelihoods.array().exp();

    double lSum = likelihoods.sum();
    logLikeSum += logLikelihoods.sum();

    particles.appendParticles(latestParticles); //particles at t

    Eigen::VectorXd weights = likelihoods/lSum;
    std::vector<double> weightsVec(weights.data(), weights.data() + nParticles);
    
    particles.resampleParticles(weightsVec, loopSeed);
  }

  logLikeSum /= T*nParticles;

  return logLikeSum;
}
