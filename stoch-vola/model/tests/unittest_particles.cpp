#include <vector>

#include "gtest/gtest.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"

TEST(Particles, DefaultConstructor) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles<double> p(initial_particles);
  EXPECT_EQ(p.getParticleCount(), 3);
  EXPECT_EQ(p.getParticleLength(), 1);
}

TEST(Particles, RandomNormalConstructor) {
  NormalDistribution nd(0.0, 1.0);
  Particles<double> first(nd, 10, 123);

  std::vector<double> samples = nd.sample(10, 123);
  Particles<double> second(samples);

  EXPECT_TRUE(first == second);
}

TEST(Particles, transformParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles<double> p(initial_particles);
  p.applyTransformation([](double v) {return v*2.0;});

  std::vector<double> target_particles = {2.0, 4.0, 6.0};
  Particles<double> pt(target_particles);

  EXPECT_TRUE(p == pt);
}

TEST(Particles, reduceParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles<double> p(initial_particles);
  std::vector<double> means = p.reduceParticles([](std::vector<double> vec){
        double result = 0;
        for (auto const& v : vec) {
          result += v;
        }
        return result / vec.size();
      });
  
  EXPECT_EQ(means.size(), 1);
  EXPECT_EQ(means[0], 2.0);
}

TEST(Particles, appendParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles<double> p(initial_particles);
  p.appendParticles(initial_particles);
  p.applyTransformation([](double v) {return v*2.0;});

  std::vector<double> test_particles = {2.0, 4.0, 6.0};
  Particles<double> pt(test_particles);
  pt.appendParticles(test_particles);

  EXPECT_TRUE(p == pt);
}

TEST(Particles, resampleParticles) {
  std::vector<double> initial_particles = {1.0, 2.0, 3.0};
  Particles<double> p(initial_particles);

  std::vector<double> weights ={1.0, 0.0, 0.0};
  p.resampleParticles(weights);

  std::vector<double> test_particles = {1.0, 1.0, 1.0};
  Particles<double> pt(test_particles);

  EXPECT_TRUE(p == pt);
}
