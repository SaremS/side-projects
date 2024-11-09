#ifndef NORMAL_DISTRIBUTION_H
#define NORMAL_DISTRIBUTION_H

#include <random>
#include <vector>
#include <cmath>

#include <Eigen/Dense>

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


class IndependentVectorNormal {
  private:
    Eigen::VectorXd means_;
    Eigen::VectorXd stdDevs_;
    mutable std::mt19937 generator_;

  public:
    IndependentVectorNormal(Eigen::VectorXd means, Eigen::VectorXd stdDevs);
    IndependentVectorNormal(double mean, double stdDev, unsigned int n);

    void setMeans(Eigen::VectorXd means);
    void setStdDevs(Eigen::VectorXd stdDevs);
    Eigen::VectorXd getMeans() const;
    Eigen::VectorXd getStdDevs() const;

    Eigen::VectorXd pdfs(const Eigen::VectorXd& x) const;
    Eigen::VectorXd pdfs(const double& x) const;

    Eigen::VectorXd logLikelihoods(const Eigen::VectorXd& x) const;
    Eigen::VectorXd logLikelihoods(const double& x) const;
    Eigen::MatrixXd colWiseLogLikelihoods(const Eigen::MatrixXd& x) const;


    Eigen::VectorXd sample(unsigned int seed = 123) const;
};


#endif


