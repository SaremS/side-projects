#include <random>
#include <vector>
#include <cmath>
#include <stdexcept>

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

