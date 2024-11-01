#include <vector>
#include <stdexcept>
#include <random>
#include <functional>

#include "statistics/particles.h"
#include "statistics/normal_distribution.h"


template <typename T>
Particles<T>::Particles(std::vector<T> initial_particles) {
  particleCount_ = initial_particles.size();
  for (int i=0; i<particleCount_; i++) {
    std::vector<T> temp = {initial_particles[i]};
    particles_.push_back(temp);
  }
  particleLength_ = 1;
}

template <typename T>
Particles<T>::Particles(std::vector<std::vector<T>> initial_particles) {
  particles_ = initial_particles;
  particleCount_ = initial_particles.size();
  particleLength_ = initial_particles[0].size();
}

template <typename T>
bool Particles<T>::operator==(const Particles<T>& other) const {
  std::vector<std::vector<T>> left = particles_; 
  std::vector<std::vector<T>> right = other.particles_;

  unsigned int particleCount = particleCount_;
  unsigned int particleLength = particleLength_;

  if (particleCount!=other.particleCount_ || particleLength!=other.particleLength_) {
    return false;
  }

  for (int i=0; i<particleCount; i++) {
    for (int j=0; j<particleLength; j++) {
      if (left[i][j] != right[i][j]) {
        return false;
      }
    }
  }

  return true;
}

template <typename T>
void Particles<T>::applyTransformation(T (*func)(T)) {
  std::vector<std::vector<T>> new_particles;
  for (int i=0; i<particleCount_; i++) {
    std::vector<T> temp;
    for (int j=0; j<particleLength_; j++) {
      temp.push_back(func(particles_[i][j]));
    }
    new_particles.push_back(temp);
  }
  particles_ = new_particles;
}

template <typename T>
std::vector<T> Particles<T>::reduceParticles(const std::function<T(std::vector<T>)>& func) const {
  std::vector<T> result;

  for (int i=0; i<particleLength_; i++) {
    std::vector<T> temp;
    for (int j=0; j<particleCount_; j++) {
      temp.push_back(particles_[j][i]);
    }
    result.push_back(func(temp));
  }
  return result;
}

template <typename T>
std::vector<T> Particles<T>::reduceTraces(const std::function<T(std::vector<T>)>& func) const {
  std::vector<T> result;

  for (int i=0; i<particleCount_; i++) {
    std::vector<T> temp;
    for (int j=0; j<particleLength_; j++) {
      temp.push_back(particles_[i][j]);
    }
    result.push_back(func(temp));
  }
  return result;
}

template <typename T>
void Particles<T>::appendParticles(std::vector<T> new_particles) {
  if (new_particles.size() != particleCount_) {
    throw std::invalid_argument("Number of new particles must be equal to the number of particles in the object.");
  }
  for (int i=0; i<particleCount_; i++) {
    particles_[i].push_back(new_particles[i]);
  }
  particleLength_ += 1;
}

template <typename T>
void Particles<T>::resampleParticles(const std::vector<double>& weights, const unsigned int& seed) {
  if (weights.size() != particleCount_) {
    throw std::invalid_argument("Number of weights must be equal to the number of particles in the object.");
  }
  setSeed(seed);
  std::discrete_distribution<int> distribution(weights.begin(), weights.end());
  std::vector<std::vector<T>> new_particles;
  for (int i=0; i<particleCount_; i++) {
    int index = distribution(generator_);
    new_particles.push_back(particles_[index]);
  }
  particles_ = new_particles;
}

template <typename T>
std::vector<T> Particles<T>::getLatestParticlesAsVector() const {
  std::vector<T> result;
  for (int i=0; i<particleCount_; i++) {
    result.push_back(particles_[i][particleLength_-1]);
  }
  return result;
}

template <typename T>
Particles<T> Particles<T>::getParticlesWithoutInit() const {
  if (particleLength_ == 1) {
    throw std::invalid_argument("Cannot remove initial particles.");
  }
   
  std::vector<std::vector<T>> new_particles = particles_;

  for (std::vector<T>& vec : new_particles) {
    vec.erase(vec.begin());
  }

  Particles<T> result = Particles<T>(new_particles);
  return result;
}

template <typename T>
unsigned int Particles<T>::getParticleCount() const {
  return particleCount_;
}

template <typename T>
unsigned int Particles<T>::getParticleLength() const {
  return particleLength_;
}

template <typename T>
void Particles<T>::setSeed(unsigned int seed) const {
    generator_.seed(seed);
}

template class Particles<double>;
