#include <vector>
#include <stdexcept>
#include <random>
#include <functional>
#include <Eigen/Dense>

#include <statistics/particles.h>
#include <statistics/normal_distribution.h>


Particles::Particles(const std::vector<double>& initialParticles, const unsigned int& particleLength) {
  particleCount_ = initialParticles.size();
  particleLength_ = particleLength;
  particles_ = Eigen::MatrixXd::Zero(particleLength, particleCount_);
  particles_.row(0) = Eigen::Map<const Eigen::RowVectorXd>(initialParticles.data(), initialParticles.size());
}

Particles::Particles(const std::vector<std::vector<double>>& initialParticles, const unsigned int& particleLength) {
  if (initialParticles.size() > particleLength) {
    throw std::invalid_argument("Initial particles cannot be longer than particle length.");
  }
  particleCount_ = initialParticles[0].size();
  particleLength_ = particleLength;

  particles_ = Eigen::MatrixXd::Zero(particleLength, particleCount_);
  for (int col = 0; col < particleCount_; ++col) {
      int len = particleLength_;  
      for (int row = 0; row < len && row < particleLength_; ++row) {
          particles_(row, col) = initialParticles[col][row];
      }
  }
}

Particles::Particles(const NormalDistribution& dist, const unsigned int& nParticles,
          const unsigned int& particleLength,
          const unsigned int& seed) {
  particleCount_ = nParticles;
  particleLength_ = particleLength;
  particles_ = Eigen::MatrixXd::Zero(particleLength, particleCount_);
  std::vector<double> samples = dist.sample(nParticles, seed);
  particles_.row(0) = Eigen::Map<const Eigen::RowVectorXd>(samples.data(), samples.size());
}

bool Particles::operator==(const Particles& other) const {
  Eigen::MatrixXd left = particles_; 
  Eigen::MatrixXd right = other.particles_;

  unsigned int particleCount = particleCount_;
  unsigned int particleLength = particleLength_;

  if (particleCount!=other.particleCount_ || particleLength!=other.particleLength_) {
    return false;
  }

  return left.isApprox(right);
}
/*
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
std::vector<std::vector<T>> Particles<T>::getParticlesAsNestedVector() const {
  return particles_;
}
*/
unsigned int Particles::getParticleCount() const {
  return particleCount_;
}

unsigned int Particles::getParticleLength() const {
  return particleLength_;
}

void Particles::setSeed(const unsigned int& seed) const {
    generator_.seed(seed);
}

