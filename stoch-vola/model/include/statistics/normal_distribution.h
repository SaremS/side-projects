#ifndef NORMAL_DISTRIBUTION_H
#define NORMAL_DISTRIBUTION_H

#include <random>
#include <vector>
#include <cmath>

class NormalDistribution {
  private:
    double mean_;
    double std_dev_;
    mutable std::mt19937 generator_;  

    void setSeed(unsigned int seed) const;

  public:
    NormalDistribution(double mean = 0.0, double std_dev = 1.0);

    void setMean(double mean);
    void setStdDev(double std_dev);
    double getMean() const;
    double getStdDev() const;

    double pdf(double x) const;

    double logLikelihood(double x) const;

    std::vector<double> sample(size_t n, unsigned int seed = 123) const;
};

#endif
