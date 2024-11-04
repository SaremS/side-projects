#ifndef PARTICLES_H
#define PARTICLES_H

#include <random>
#include <vector>
#include <cmath>
#include <functional>

#include <Eigen/Dense>

#include "normal_distribution.h"

class Particles {
  private:
    Eigen::MatrixXd particles_;
    unsigned int particleCount_;
    unsigned int particleLength_;
    unsigned int currentRow_;
    mutable std::mt19937 generator_;
    void setSeed(const unsigned int& seed) const;

  public:
    Particles(const std::vector<double>& initialParticles, const unsigned int& particleLength = 1);
    Particles(const Eigen::VectorXd& initialParticles, const unsigned int& particleLength = 1);
    Particles(const Eigen::MatrixXd& initialParticles, const unsigned int& particleLength = 1);
    Particles(const std::vector<std::vector<double>>& initialParticles, const unsigned int& particleLength = 1);
    Particles(const NormalDistribution& dist, const unsigned int& nParticles,
              const unsigned int& particleLength = 1,
              const unsigned int& seed = 123);


    void appendParticles(const Eigen::VectorXd& newParticles);
    void resampleParticles(const std::vector<double>& weights, const unsigned int& seed = 123);
    void applyTransformation(const std::function<double(double)>& func);
    Eigen::VectorXd getLatestParticles() const;
    Eigen::VectorXd reduceParticles(const std::function<double(const Eigen::VectorXd&)>& func) const; //reduce over particles
    Eigen::VectorXd reduceTraces(const std::function<double(const Eigen::VectorXd&)>& func) const; //reduce over all elements of a particle
    Eigen::MatrixXd getParticlesAsEigenMatrix() const;
    Particles getParticlesWithoutInit() const;
 
    bool operator==(const Particles& other) const;

    unsigned int getParticleCount() const;
    unsigned int getParticleLength() const;
};

#endif
