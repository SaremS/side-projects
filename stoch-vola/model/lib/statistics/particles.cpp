#include <vector>
#include <stdexcept>
#include <random>
#include <functional>
#include <Eigen/Dense>

#include <statistics/particles.h>
#include <statistics/normal_distribution.h>


Particles::Particles(const Eigen::VectorXd& initialParticles, const unsigned int& particleLength) {
  particleCount_ = initialParticles.size();
  particleLength_ = particleLength;
  currentRow_ = 0;
  particles_ = Eigen::MatrixXd::Zero(particleLength, particleCount_);
  particles_.row(0) = initialParticles;
}

Particles::Particles(const Eigen::MatrixXd& initialParticles, const unsigned int& particleLength) {
  if (initialParticles.rows() > particleLength) {
    throw std::invalid_argument("Initial particles cannot be longer than particle length.");
  }

  particleCount_ = initialParticles.cols();
  particleLength_ = particleLength;
  currentRow_ = 0;
  particles_ = Eigen::MatrixXd::Zero(particleLength, particleCount_);
  particles_.topRows(initialParticles.rows()) = initialParticles;
}


Particles::Particles(const std::vector<double>& initialParticles, const unsigned int& particleLength) {
  particleCount_ = initialParticles.size();
  particleLength_ = particleLength;
  currentRow_ = 0;
  particles_ = Eigen::MatrixXd::Zero(particleLength, particleCount_);
  particles_.row(0) = Eigen::Map<const Eigen::RowVectorXd>(initialParticles.data(), initialParticles.size());
}


Particles::Particles(const std::vector<std::vector<double>>& initialParticles, const unsigned int& particleLength) {
  if (initialParticles.size() > particleLength) {
    throw std::invalid_argument("Initial particles cannot be longer than particle length.");
  }
  particleCount_ = initialParticles.size();
  particleLength_ = particleLength;
  currentRow_ = initialParticles[0].size() - 1;

  particles_ = Eigen::MatrixXd::Zero(particleLength_, particleCount_);
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
  currentRow_ = 0;
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


void Particles::applyTransformation(const std::function<double(double)>& func) {
  particles_ = particles_.unaryExpr(func);
}


Eigen::VectorXd Particles::reduceParticles(const std::function<double(const Eigen::VectorXd&)>& func) const {
  Eigen::VectorXd result(particleLength_);
  for (int row = 0; row < particleLength_; ++row) {
      result[row] = func(particles_.row(row));
  }

  return result;
}

Eigen::VectorXd Particles::reduceTraces(const std::function<double(const Eigen::VectorXd&)>& func) const {
  Eigen::VectorXd result(particleCount_);
  for (int col = 0; col < particleCount_; ++col) {
      result[col] = func(particles_.col(col));
  }

  return result;
}


void Particles::appendParticles(const Eigen::VectorXd& newParticles) {
  if (currentRow_ >= particleLength_ - 1) {
    throw std::invalid_argument("Cannot append more particles.");
  }
  if (newParticles.size() != particleCount_) {
    throw std::invalid_argument("Number of new particles must be equal to the number of particles in the object.");
  }

  particles_.row(currentRow_+1) = newParticles;
  currentRow_++;
}

Eigen::VectorXd Particles::getLatestParticles() const {
  Eigen::VectorXd result = particles_.row(currentRow_);
  return result;
}

Eigen::MatrixXd Particles::getParticlesAsEigenMatrix() const {
  return particles_;
}

void Particles::resampleParticles(const std::vector<double>& weights, const unsigned int& seed) {
  if (weights.size() != particleCount_) {
    throw std::invalid_argument("Number of weights must be equal to the number of particles in the object.");
  }
  setSeed(seed);
  std::discrete_distribution<int> distribution(weights.begin(), weights.end());

  Eigen::MatrixXd newParticles = Eigen::MatrixXd::Zero(particleLength_, particleCount_);

  for (int col = 0; col < particleCount_; ++col) {
    int index = distribution(generator_);
    newParticles.col(col) = particles_.col(index);
  }

  particles_ = newParticles;
}


Particles Particles::getParticlesWithoutInit() const {
  if (particleLength_ == 1) {
    throw std::invalid_argument("Cannot remove initial particles.");
  }
   
  //return from second row to end 
  Eigen::MatrixXd newParticles = particles_.bottomRows(particleLength_-1);
  Particles result(newParticles, particleLength_-1);

  return result;
}


unsigned int Particles::getParticleCount() const {
  return particleCount_;
}

unsigned int Particles::getParticleLength() const {
  return particleLength_;
}

void Particles::setSeed(const unsigned int& seed) const {
    generator_.seed(seed);
}

