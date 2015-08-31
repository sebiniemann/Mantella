#include <mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm/particleSwarmOptimisation.hpp>

// C++ standard library
#include <random>

// Mantella
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/random.hpp>

namespace mant {
  ParticleSwarmOptimisation::ParticleSwarmOptimisation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const arma::uword populationSize)
    : PopulationBasedOptimisationAlgorithm(optimisationProblem, populationSize),
      localBestObjectiveValues_(populationSize_) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(populationSize_), 3.0));
    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
    setMaximalSwarmConvergence(0.05); // TODO Check value within the paper
  }

  void ParticleSwarmOptimisation::optimiseImplementation() {
    initialiseSwarm();

    arma::Mat<arma::uword> topology_ = getRandomNeighbourhoodTopology();
    bool randomizeTopology_ = false;
    while(!isFinished() && !isTerminated()) {
      if (randomizeTopology_) {
        topology_ = getRandomNeighbourhoodTopology();
        randomizeTopology_ = false;
      }

      const arma::Col<arma::uword>& permutation = getRandomPermutation(populationSize_);
      for (arma::uword n = 0; n < populationSize_; ++n) {
        ++numberOfIterations_;

        arma::uword particleIndex_ = permutation(n);
        arma::Col<double> particle_ = particles_.col(particleIndex_);

        arma::uword neighbourhoodBestParticleIndex_;
        arma::Col<arma::uword> neighbourhoodParticlesIndecies_ = arma::find(topology_.col(particleIndex_));
        static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies_)).min(neighbourhoodBestParticleIndex_);
        neighbourhoodBestParticleIndex_ = neighbourhoodParticlesIndecies_(neighbourhoodBestParticleIndex_);

        if (neighbourhoodBestParticleIndex_ == particleIndex_) {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_)) / 2.0;
        } else {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex_) - particle_)) / 3.0;
        }

        arma::Col<double> velocityCandidate = maximalAcceleration_ * getAcceleration() *  velocities_.col(particleIndex_) + getVelocity() * arma::norm(attractionCenter_) + attractionCenter_;
        arma::Col<double> solutionCandidate = particle_ + velocityCandidate;

        const arma::Col<arma::uword>& belowLowerBound = arma::find(solutionCandidate < getLowerBounds());
        const arma::Col<arma::uword>& aboveUpperBound = arma::find(solutionCandidate > getUpperBounds());

        velocityCandidate.elem(belowLowerBound) *= -0.5;
        velocityCandidate.elem(aboveUpperBound) *= -0.5;

        solutionCandidate.elem(belowLowerBound) = getLowerBounds().elem(belowLowerBound);
        solutionCandidate.elem(aboveUpperBound) = getUpperBounds().elem(aboveUpperBound);

        velocities_.col(particleIndex_) = velocityCandidate;
        particles_.col(particleIndex_) = solutionCandidate;

        const double objectiveValue = getObjectiveValue(solutionCandidate);

        if (objectiveValue < localBestObjectiveValues_(particleIndex_)) {
          localBestObjectiveValues_(particleIndex_) = objectiveValue;
          localBestSolutions_.col(particleIndex_) = solutionCandidate;
        }

        if (objectiveValue < bestObjectiveValue_) {
          bestObjectiveValue_ = objectiveValue;
          bestParameter_ = solutionCandidate;
        } else {
          randomizeTopology_ = true;
        }

        if (isFinished() || isTerminated()) {
          break;
        }
      }

      if (static_cast<arma::Row<double>>(arma::stddev(particles_, 1)).max() < maximalSwarmConvergence_) {
        initialiseSwarm();
      }
    }
  }

  void ParticleSwarmOptimisation::initialiseSwarm() {
    particles_ = arma::randu<arma::Mat<double>>(numberOfDimensions_, populationSize_);
    particles_.each_col() %= getUpperBounds() - getLowerBounds();
    particles_.each_col() += getLowerBounds();

    velocities_ = arma::randu<arma::Mat<double>>(numberOfDimensions_, populationSize_);
    velocities_.each_col() %= getUpperBounds() - getLowerBounds();
    velocities_.each_col() += getLowerBounds();
    velocities_ -= particles_;

    localBestSolutions_ = particles_;

    for (arma::uword n = 0; n < populationSize_; ++n) {
      ++numberOfIterations_;

      arma::Col<double> localBestSolution = localBestSolutions_.col(n);
      double localBestObjectiveValue = getObjectiveValue(localBestSolution);
      localBestObjectiveValues_(n) = localBestObjectiveValue;

      if (localBestObjectiveValue < bestObjectiveValue_) {
        bestParameter_ = localBestSolution;
        bestObjectiveValue_ = localBestObjectiveValue;
      }

      if (isFinished() || isTerminated()) {
        break;
      }
    }
  }

  double ParticleSwarmOptimisation::getAcceleration() {
    return std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  arma::Col<double> ParticleSwarmOptimisation::getVelocity() {
    return arma::normalise(arma::randn<arma::Col<double>>(numberOfDimensions_)) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  arma::Mat<arma::uword> ParticleSwarmOptimisation::getRandomNeighbourhoodTopology() {
    arma::Mat<arma::uword>topology = (arma::randu<arma::Mat<double>>(populationSize_, populationSize_) <= neighbourhoodProbability_);
    topology.diag().ones();

    return topology;
  }

  void ParticleSwarmOptimisation::setNeighbourhoodProbability(
      const double neighbourhoodProbability) {
    verify(neighbourhoodProbability >= 0 && neighbourhoodProbability <= 1, "NeighbourhoodProbability must be a value between 0 and 1");
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void ParticleSwarmOptimisation::setMaximalAcceleration(
      const double maximalAcceleration) {
    maximalAcceleration_ = maximalAcceleration;
  }

  void ParticleSwarmOptimisation::setMaximalLocalAttraction(
      const double maximalLocalAttraction) {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  void ParticleSwarmOptimisation::setMaximalGlobalAttraction(
      const double maximalGlobalAttraction) {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  void ParticleSwarmOptimisation::setMaximalSwarmConvergence(
      const double maximalSwarmConvergence) {
    maximalSwarmConvergence_ = maximalSwarmConvergence;
  }

  std::string ParticleSwarmOptimisation::toString() const {
    return "particle_swarm_optimisation";
  }
}
