#include <vector>
#include <algorithm>
#include <functional>

#include "gtest/gtest.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"


TEST(StochasticVolatility_Particles, EigenVectorConstructor) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Eigen::VectorXd initial_particles_eigen = Eigen::Map<const Eigen::VectorXd>(initial_particles.data(), initial_particles.size());
  Particles p(initial_particles_eigen);
  EXPECT_EQ(p.getParticleCount(), 3);
  EXPECT_EQ(p.getParticleLength(), 1);
}

TEST(StochasticVolatility_Particles, EigenMatrixConstructor) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Eigen::MatrixXd initial_particles_eigen = Eigen::Map<const Eigen::MatrixXd>(initial_particles.data(), 1, initial_particles.size());
  Particles p(initial_particles_eigen);
  EXPECT_EQ(p.getParticleCount(), 3);
  EXPECT_EQ(p.getParticleLength(), 1);
}

TEST(StochasticVolatility_Particles, DefaultConstructor) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles);
  EXPECT_EQ(p.getParticleCount(), 3);
  EXPECT_EQ(p.getParticleLength(), 1);
}


TEST(StochasticVolatility_Particles, RandomNormalConstructor) {
  NormalDistribution nd(0.0, 1.0);
  Particles first(nd, 10, 10, 123);

  std::vector<double> samples = nd.sample(10, 123);
  Particles second(samples,10);

  EXPECT_TRUE(first == second);
}

TEST(StochasticVolatility_Particles, IndependentVectorNormalConstructor) {
  Eigen::VectorXd means(3);
  means << 1.0, 2.0, 3.0;
  Eigen::VectorXd stdDevs(3);
  stdDevs << 0.1, 0.2, 0.3;
  IndependentVectorNormal ivn(means, stdDevs);
  Particles p(ivn, 10, 123);
  Eigen::VectorXd samples = ivn.sample(123);
  Particles pt(samples, 10);
  EXPECT_TRUE(p == pt);

  Eigen::VectorXd samples2 = ivn.sample(124);
  Particles ptt(samples2, 10);
  EXPECT_FALSE(p == ptt);
}



TEST(StochasticVolatility_Particles, transformParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles);
  p.applyTransformation([](double v) {return v*2.0;});

  std::vector<double> target_particles = {2.0, 4.0, 6.0};
  Particles pt(target_particles);

  EXPECT_TRUE(p == pt);
}


TEST(StochasticVolatility_Particles, reduceParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles, 1);
  Eigen::VectorXd means = p.reduceParticles([](Eigen::VectorXd vec){
        return vec.mean();
      }
  );
  
  EXPECT_EQ(means.size(), 1);
  EXPECT_EQ(means(0,0), 2.0);
}

TEST(StochasticVolatility_Particles, reduceTraces) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles);
  Eigen::VectorXd means = p.reduceTraces([](Eigen::VectorXd vec){
        return vec.mean();
      }
  );

  EXPECT_EQ(means.size(), 3);
  EXPECT_EQ(means(0), 1.0);
  EXPECT_EQ(means(1), 2.0);
  EXPECT_EQ(means(2), 3.0);
}


TEST(StochasticVolatility_Particles, appendParticles) {
  std::vector<double> initialParticles = {1.0, 2.0, 3.0};
  Eigen::VectorXd initialParticlesEigen = Eigen::Map<const Eigen::VectorXd>(initialParticles.data(), initialParticles.size());
  Particles p(initialParticles, 2);
  p.appendParticles(initialParticlesEigen);
  p.applyTransformation([](double v) {return v*2.0;});

  std::vector<double> testParticles = {2.0, 4.0, 6.0};
  Eigen::VectorXd testParticlesEigen = Eigen::Map<const Eigen::VectorXd>(testParticles.data(), testParticles.size());
  Particles pt(testParticles, 2);
  pt.appendParticles(testParticlesEigen);

  EXPECT_TRUE(p == pt);
}

TEST(StochasticVolatility_Particles, getLatestParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles);
  Eigen::VectorXd particles = p.getLatestParticles();
  EXPECT_TRUE(particles(0) == 1.0);
  EXPECT_TRUE(particles(1) == 2.0);
  EXPECT_TRUE(particles(2) == 3.0);
}


TEST(StochasticVolatility_Particles, GetParticlesAsEigenMatrix) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles);
  Eigen::MatrixXd particles = p.getParticlesAsEigenMatrix();

  EXPECT_TRUE(particles(0,0) == initial_particles[0]);
  EXPECT_TRUE(particles(0,1) == initial_particles[1]);
  EXPECT_TRUE(particles(0,2) == initial_particles[2]);
}


TEST(StochasticVolatility_Particles, resampleParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles p(initial_particles);

  std::vector<double> weights ={1.0, 0.0, 0.0};
  p.resampleParticles(weights);

  std::vector<double> test_particles = {1.0, 1.0, 1.0};
  Particles pt(test_particles);

  EXPECT_TRUE(p == pt);
}
