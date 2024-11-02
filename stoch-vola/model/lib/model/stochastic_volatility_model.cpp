#include <vector>
#include <cmath>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <span>
#include <algorithm>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "model/stochastic_volatility_model.h"
#include "statistics/normal_distribution.h"
#include "statistics/particles.h"


namespace py = pybind11;

PYBIND11_MODULE(stochastic_volatility_model,m) {
	py::class_<StochasticVolatilityModel>(m, "StochasticVolatilityModel")
		.def(py::init<double, double, double>(),
				py::arg("mu") = 0.0,
				py::arg("phi") = 0.0,
				py::arg("sigma") = 0.0)
		.def("particleFilter", &StochasticVolatilityModel::particleFilter)
    .def("logLikelihood", &StochasticVolatilityModel::logLikelihood);

	py::class_<Particles<double>>(m, "Particles")
		.def("get_particles", &Particles<double>::getParticlesAsNestedVector);
}


StochasticVolatilityModel::StochasticVolatilityModel(double mu, double phi, double sigma) : mu_(mu), phi_(phi), sigma_(sigma) {}

Particles<double> StochasticVolatilityModel::particleFilter(const std::vector<double>& y, const unsigned int& nParticles, const unsigned int& seed) {
  unsigned int T = y.size();

  double mu = mu_;
  double phi = std::tanh(phi_);
  double sigma = std::exp(sigma_);

  Particles<double> particles = Particles<double>(NormalDistribution(mu, sigma), nParticles, seed);

  int loopSeed = seed; //unique seed for each (nested) loop iteration

  for (int t=1; t<=T; t++) {
    std::vector<double> latestParticles = particles.getLatestParticlesAsVector(); //particles at t-1
    std::vector<double> weights(nParticles); //weights can be calculated inside loop as well                
    double weightSum = 0.0; //to divide later on

    std::vector<std::thread> threads;

    std::vector<int> seeds;
    for (int i=0; i<nParticles; i++) {
      seeds.push_back(loopSeed + i);
      loopSeed += 1;
    }

    unsigned int numThreads = std::max(nParticles/10,static_cast<unsigned int>(1));
    unsigned int batchSize = (nParticles + numThreads - 1) / numThreads;

    for (size_t i = 0; i < numThreads; ++i) {
        unsigned int start = i * batchSize;
        unsigned int end = std::min(start + batchSize, nParticles);

        if (start < nParticles) { 
            std::span<double> particleSpan(latestParticles.begin() + start, end - start);
            std::span<double> weightSpan(weights.begin() + start, end - start);
            std::span<int> seedspan(seeds.begin() + start, end - start);


            threads.emplace_back(&StochasticVolatilityModel::process_particle, this,
                              particleSpan, weightSpan,
                              std::ref(weightSum), std::cref(y[t-1]), seedspan,
                              std::cref(mu), std::cref(phi), std::cref(sigma));
        }
    }


    for (auto& thread : threads) {
        thread.join();
    }

    particles.appendParticles(latestParticles); //particles at t

    if (weightSum > 0.0) { //do nothing in the degenerate case
      for (double& w : weights) {
        w /= weightSum;
      }
      
      particles.resampleParticles(weights, loopSeed);
    }
  }

  return particles.getParticlesWithoutInit();
}

double StochasticVolatilityModel::logLikelihood(const std::vector<double>& y, const unsigned int& nParticles, const unsigned int& seed) {
  Particles<double> particles = particleFilter(y, nParticles, seed);

  std::function<double(std::vector<double>)> func = [y](std::vector<double> trace) {
    unsigned int T = trace.size();
    
    double likelihood = 0.0;
    for (int t=0; t<T; t++) {
      double p = trace[t];
      likelihood += NormalDistribution(0.0, std::exp(p/2.0)).logLikelihood(y[t]);
    }
    return likelihood/T;
  };

  std::vector<double> loglikelihoodPerParticle = particles.reduceTraces(func);

  double loglikelihood = 0.0;
  for (double& ll : loglikelihoodPerParticle) {
    loglikelihood += ll/nParticles;
  }

  return loglikelihood;
}

void StochasticVolatilityModel::process_particle(std::span<double> particles, std::span<double> weights, double& weightSum, const double& observation,
    const std::span<int> loopSeeds,
    const double& mu, const double& phi, const double& sigma) {
    
    int N = particles.size();
    int localWeightSum = 0.0;

    for (int i=0; i<N; i++) {
      particles[i] = mu + phi * (particles[i] - mu) + NormalDistribution(0.0, sigma).sample(1, loopSeeds[i])[0];
      double likelihood = NormalDistribution(0.0, std::exp(particles[i] / 2.0)).pdf(observation);

      weights[i] = likelihood;
      localWeightSum += likelihood;
    }
    
    std::lock_guard<std::mutex> lock(particle_mutex_);
    weightSum += localWeightSum;
}
