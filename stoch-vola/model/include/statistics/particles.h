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
    mutable std::mt19937 generator_;
    void setSeed(const unsigned int& seed) const;

  public:
    Particles(const std::vector<double>& initialParticles, const unsigned int& particleLength = 1);
    Particles(const std::vector<std::vector<double>>& initial_particles, const unsigned int& particleLength = 1);
    Particles(const NormalDistribution& dist, const unsigned int& nParticles,
              const unsigned int& particleLength = 1,
              const unsigned int& seed = 123);


    void appendParticles(const std::vector<double>& new_particles);
    void resampleParticles(const std::vector<double>& weights, const unsigned int& seed = 123);
    void applyTransformation(const std::function<double(double)>& func);
    std::vector<double> reduceParticles(const std::function<double(std::vector<double>)>& func) const; //reduce over particles
    std::vector<double> reduceTraces(const std::function<double(std::vector<double>)>& func) const; //reduce over all elements of a particle
    bool operator==(const Particles& other) const;
    std::vector<double> getLatestParticlesAsVector() const;
    Particles getParticlesWithoutInit() const;

    std::vector<std::vector<double>> getParticlesAsNestedVector() const;
    Eigen::MatrixXd getParticlesAsMatrix() const;

    unsigned int getParticleCount() const;
    unsigned int getParticleLength() const;
};

#endif
