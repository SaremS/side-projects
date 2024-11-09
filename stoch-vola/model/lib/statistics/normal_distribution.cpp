#include <random>
#include <vector>
#include <cmath>
#include <stdexcept>

#include <Eigen/Dense>
#include <EigenRand/EigenRand>

#include "statistics/normal_distribution.h"


NormalDistribution::NormalDistribution(double mean, double std_dev) : mean_(mean), std_dev_(std_dev) {
  if (std_dev_ <= 0) {
    throw std::invalid_argument("Standard deviation must be greater than zero.");
  }
}

void NormalDistribution::setMean(double mean) {
    mean_ = mean;
}

void NormalDistribution::setStdDev(double std_dev) {
    std_dev_ = std_dev;
}

double NormalDistribution::getMean() const {
    return mean_;
}

double NormalDistribution::getStdDev() const {
    return std_dev_;
}

double NormalDistribution::pdf(double x) const {
    return 1 / (std_dev_ * std::sqrt(2 * M_PI)) * std::exp(-0.5 * std::pow((x - mean_) / std_dev_, 2));
}

double NormalDistribution::logLikelihood(double x) const {
    return -0.5 * std::log(2 * M_PI) - std::log(std_dev_) - 0.5 * std::pow((x - mean_) / std_dev_, 2);
}

std::vector<double> NormalDistribution::sample(size_t n, unsigned int seed) const {
    setSeed(seed);
    std::normal_distribution<double> distribution(mean_, std_dev_);
    std::vector<double> samples(n);
    for (size_t i = 0; i < n; ++i) {
        samples[i] = distribution(generator_);
    }
    return samples;
}

void NormalDistribution::setSeed(unsigned int seed) const {
    generator_.seed(seed);
}



IndependentVectorNormal::IndependentVectorNormal(Eigen::VectorXd means, Eigen::VectorXd stdDevs) : means_(means), stdDevs_(stdDevs) {
  if (means_.size() != stdDevs_.size()) {
    throw std::invalid_argument("Means and standard deviations must have the same length.");
  }
  if (stdDevs_.minCoeff() <= 0) {
    throw std::invalid_argument("Standard deviations must be greater than zero.");
  }
}

IndependentVectorNormal::IndependentVectorNormal(double mean, double stdDev, unsigned int n) : means_(Eigen::VectorXd::Constant(n, mean)), stdDevs_(Eigen::VectorXd::Constant(n, stdDev)) {
  if (stdDevs_.minCoeff() <= 0) {
    throw std::invalid_argument("Standard deviation must be greater than zero.");
  }
}

void IndependentVectorNormal::setMeans(Eigen::VectorXd means) {
  means_ = means;
}

void IndependentVectorNormal::setStdDevs(Eigen::VectorXd stdDevs) {
  if (stdDevs.minCoeff() <= 0) {
    throw std::invalid_argument("Standard deviations must be greater than zero.");
  }

  stdDevs_ = stdDevs;
}

Eigen::VectorXd IndependentVectorNormal::getMeans() const {
  return means_;
}

Eigen::VectorXd IndependentVectorNormal::getStdDevs() const {
  return stdDevs_;
}

Eigen::VectorXd IndependentVectorNormal::pdfs(const Eigen::VectorXd& x) const {
  if (x.size() != means_.size()) {
    throw std::invalid_argument("Input vector must have the same length as means.");
  }

  Eigen::VectorXd leftQuotient = stdDevs_ * std::sqrt(2.0*M_PI);
  Eigen::VectorXd leftFactor = 1.0 / leftQuotient.array();

  Eigen::VectorXd xMuDiff = x.array() - means_.array();
  Eigen::VectorXd rightFactor = (-0.5 * ((xMuDiff.array() / stdDevs_.array()).array().pow(2))).array().exp();

  return leftFactor.array() * rightFactor.array();
}

Eigen::VectorXd IndependentVectorNormal::pdfs(const double& x) const {
  Eigen::VectorXd leftQuotient = stdDevs_ * std::sqrt(2.0*M_PI);
  Eigen::VectorXd leftFactor = 1.0 / leftQuotient.array();

  Eigen::VectorXd xMuDiff = x - means_.array();
  Eigen::VectorXd rightFactor = (-0.5 * ((xMuDiff.array() / stdDevs_.array()).array().pow(2))).array().exp();

  return leftFactor.array() * rightFactor.array();
}

Eigen::VectorXd IndependentVectorNormal::logLikelihoods(const Eigen::VectorXd& x) const {
  if (x.size() != means_.size()) {
    throw std::invalid_argument("Input vector must have the same length as means.");
  }
  Eigen::VectorXd leftFactor = -0.5 * std::log(2 * M_PI) - stdDevs_.array().log();
  Eigen::VectorXd xMuDiff = x.array() - means_.array();
  Eigen::VectorXd rightFactor = -0.5 * ((xMuDiff.array() / stdDevs_.array()).array().pow(2));

  return leftFactor.array() + rightFactor.array();
}

Eigen::VectorXd IndependentVectorNormal::logLikelihoods(const double& x) const {
  Eigen::VectorXd leftFactor = -0.5 * std::log(2 * M_PI) - stdDevs_.array().log();
  Eigen::VectorXd xMuDiff = x - means_.array();
  Eigen::VectorXd rightFactor = -0.5 * ((xMuDiff.array() / stdDevs_.array()).array().pow(2));

  return leftFactor.array() + rightFactor.array();
}

Eigen::MatrixXd IndependentVectorNormal::colWiseLogLikelihoods(const Eigen::MatrixXd& x) const {
  Eigen::VectorXd leftFactor = -0.5 * std::log(2 * M_PI) - stdDevs_.array().log();
  Eigen::MatrixXd xMuDiff = x.colwise() - means_;
  Eigen::MatrixXd xZStandard = xMuDiff.array().colwise() / stdDevs_.array();

  Eigen::MatrixXd rightFactor = -0.5 * xZStandard.array().pow(2);
   
  return rightFactor.colwise() + leftFactor;
}


Eigen::VectorXd IndependentVectorNormal::sample(unsigned int seed) const {
  Eigen::Rand::P8_mt19937_64 rng{seed};

  int sampleSize = means_.size();
  Eigen::MatrixXd samplesMatrix = Eigen::Rand::normal<Eigen::MatrixXd>(sampleSize, 1, rng);
  Eigen::VectorXd samples = samplesMatrix.col(0);

  samples.array() *= stdDevs_.array();
  samples.array() += means_.array();

  return samples;
}

