#ifndef PARTICLES_H
#define PARTICLES_H

#include <random>
#include <vector>
#include <cmath>
#include <type_traits>

#include "normal_distribution.h"

template <typename T>
class Particles {
  private:
    std::vector<std::vector<T>> particles_; //vector of particles of given length
    unsigned int particleCount_;
    unsigned int particleLength_;
    mutable std::mt19937 generator_;
    void setSeed(unsigned int seed) const;

  public:
    Particles(std::vector<T> initial_particles);

    template <typename = std::enable_if_t<std::is_same<T, double>::value>>
    Particles(const NormalDistribution& dist, const unsigned int& nParticles, const unsigned int& seed = 123) {
      std::vector<T> samples = dist.sample(nParticles, seed);
      particleCount_ = nParticles;
      for (int i=0; i<particleCount_; i++) {
        std::vector<T> temp = {samples[i]};
        particles_.push_back(temp);
      }
      particleLength_ = 1;
    }

    void appendParticles(std::vector<T> new_particles);
    void resampleParticles(const std::vector<double>& weights, const unsigned int& seed = 123);
    void applyTransformation(T (*func)(T));
    std::vector<T> reduceParticles(T (*func)(std::vector<T>)) const;
    bool operator==(const Particles<T>& other) const;

    unsigned int getParticleCount() const;
    unsigned int getParticleLength() const;
};

#endif
