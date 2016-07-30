#include "mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/particleSwarmOptimisation.hpp"

// C++ standard library
#include <cmath>
#include <functional>
#include <limits>
#include <random>
#include <string>
#include <utility>
#include <vector>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  ParticleSwarmOptimisation::ParticleSwarmOptimisation()
      : PopulationBasedOptimisationAlgorithm() {
    setInitialisingFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            velocities_ = uniformRandomNumbers(numberOfDimensions_, populationSize_, std::uniform_real_distribution<double>(-1.0, 1.0)) - initialParameters_;

            return initialParameters_;
          },
          "Velocity initialisation"},
         {[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            localBestSolutions_ = initialParameters_;

            return initialParameters_;
          },
          "Local best solution initialisation"},
         {[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& initialParameters_) {
            localBestObjectiveValues_.set_size(populationSize_);
            localBestObjectiveValues_.fill(std::numeric_limits<double>::infinity());

            return initialParameters_;
          },
          "Local best objective values initialisation"}});

    setNextParametersFunctions(
        {{[this](
              const arma::uword numberOfDimensions_,
              const arma::mat& parameters_,
              const arma::rowvec& objectiveValues_,
              const arma::rowvec& differences_) {
            for (arma::uword n = 0; n < populationSize_; ++n) {
              if (objectiveValues_(n) < localBestObjectiveValues_(n)) {
                localBestSolutions_.col(n) = parameters_.col(n);
                localBestObjectiveValues_(n) = objectiveValues_(n);
              }
            }

            arma::mat particles(arma::size(parameters_));
            for (arma::uword n = 0; n < populationSize_; ++n) {
              const arma::vec& particle = parameters_.col(n);

              arma::vec attractionCenter = (getMaximalLocalAttraction() * (localBestSolutions_.col(n) - particle) + getMaximalGlobalAttraction() * (getBestFoundParameter() - particle)) / 3.0;
              const arma::vec& velocity = getMaximalAcceleration() * uniformRandomNumbers(numberOfDimensions_) % velocities_.col(n) + randomNeighbour(attractionCenter, 0, arma::norm(attractionCenter));

              particles.col(n) = particle + velocity;
              velocities_.col(n) = velocity;
            }

            return particles;
          },
          "Particle swarm optimisation"}});

    auto boundariesHandlingFunctions = getBoundariesHandlingFunctions();
    boundariesHandlingFunctions.push_back(
        {[this](
             const arma::mat& parameters_,
             const arma::umat& isBelowLowerBound_,
             const arma::umat& isAboveUpperBound_) {
           velocities_.elem(arma::find(isBelowLowerBound_)) *= -0.5;
           velocities_.elem(arma::find(isAboveUpperBound_)) *= -0.5;

           return parameters_;
         },
         "Halve velocity and revert direction"});
    setBoundariesHandlingFunctions(boundariesHandlingFunctions);

    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
  }

  void ParticleSwarmOptimisation::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, uniformRandomNumbers(optimisationProblem.numberOfDimensions_, populationSize_));
  }

  void ParticleSwarmOptimisation::setMaximalAcceleration(
      const double maximalAcceleration) {
    maximalAcceleration_ = maximalAcceleration;
  }

  double ParticleSwarmOptimisation::getMaximalAcceleration() const {
    return maximalAcceleration_;
  }

  void ParticleSwarmOptimisation::setMaximalLocalAttraction(
      const double maximalLocalAttraction) {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  double ParticleSwarmOptimisation::getMaximalLocalAttraction() const {
    return maximalLocalAttraction_;
  }

  void ParticleSwarmOptimisation::setMaximalGlobalAttraction(
      const double maximalGlobalAttraction) {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  double ParticleSwarmOptimisation::getMaximalGlobalAttraction() const {
    return maximalGlobalAttraction_;
  }
}
