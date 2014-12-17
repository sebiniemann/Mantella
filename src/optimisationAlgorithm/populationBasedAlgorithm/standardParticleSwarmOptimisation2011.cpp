#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/standardParticleSwarmOptimisation2011.hpp>

// C++ Standard Library
#include <cmath>
#include <random>

// HOP
#include <hop_bits/helper/random.hpp>
#include <hop_bits/helper/rng.hpp>

// TODO Update
namespace hop {
  StandardParticleSwarmOptimisation2011::StandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int& populationSize) noexcept
    : PopulationBasedAlgorithm<double>(optimisationProblem, populationSize),
      localBestObjectiveValues_(populationSize_),
      randomizeTopology_(true) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(populationSize_), 3.0));
    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
    setMaximalSwarmConvergence(0.05); // TODO Check value within the paper
  }

  void StandardParticleSwarmOptimisation2011::optimiseImplementation() noexcept {
    initialiseSwarm();

    while(!isFinished() && !isTerminated()) {
      if (randomizeTopology_) {
        topology_ = getNeighbourhoodTopology();
        randomizeTopology_ = false;
      }

      const arma::Col<arma::uword>& permutation = getRandomPermutation(populationSize_);
      for (std::size_t n = 0; n < populationSize_; ++n) {
        ++numberOfIterations_;

        particleIndex_ = permutation.at(n);
        particle_ = particles_.col(particleIndex_);

        neighbourhoodParticlesIndecies_ = arma::find(topology_.col(particleIndex_));
        static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies_)).min(neighbourhoodBestParticleIndex_);
        neighbourhoodBestParticleIndex_ = neighbourhoodParticlesIndecies_.at(neighbourhoodBestParticleIndex_);

        if (neighbourhoodBestParticleIndex_ == particleIndex_) {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_)) / 2.0;
        } else {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex_) - particle_)) / 3.0;
        }

        arma::Col<double> velocityCandidate = maximalAcceleration_ * getAcceleration() *  velocities_.col(particleIndex_) + getVelocity() * arma::norm(attractionCenter_) + attractionCenter_;
        arma::Col<double> solutionCandidate = particle_ + velocityCandidate;

        const arma::Col<arma::uword>& belowLowerBound = arma::find(solutionCandidate < optimisationProblem_->getLowerBounds());
        const arma::Col<arma::uword>& aboveUpperBound = arma::find(solutionCandidate > optimisationProblem_->getUpperBounds());

        velocityCandidate.elem(belowLowerBound) *= -0.5;
        velocityCandidate.elem(aboveUpperBound) *= -0.5;

        solutionCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
        solutionCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

        velocities_.col(particleIndex_) = velocityCandidate;
        particles_.col(particleIndex_) = solutionCandidate;

        const double& objectiveValue = optimisationProblem_->getObjectiveValue(solutionCandidate) + optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

        if (objectiveValue < localBestObjectiveValues_.at(particleIndex_)) {
          localBestObjectiveValues_.at(particleIndex_) = objectiveValue;
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

  void StandardParticleSwarmOptimisation2011::initialiseSwarm() noexcept {
    particles_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    particles_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    particles_.each_col() += optimisationProblem_->getLowerBounds();

    velocities_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    velocities_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    velocities_.each_col() += optimisationProblem_->getLowerBounds();
    velocities_ -= particles_;

    localBestSolutions_ = particles_;

    for (std::size_t n = 0; n < populationSize_; ++n) {
      ++numberOfIterations_;

      arma::Col<double> localBestSolution = localBestSolutions_.col(n);
      double localBestObjectiveValue = optimisationProblem_->getObjectiveValue(localBestSolution) + optimisationProblem_->getSoftConstraintsValue(localBestSolution);
      localBestObjectiveValues_.at(n) = localBestObjectiveValue;

      if (localBestObjectiveValue < bestObjectiveValue_) {
        bestParameter_ = localBestSolution;
        bestObjectiveValue_ = localBestObjectiveValue;
      }

      if (isFinished() || isTerminated()) {
        break;
      }
    }

    randomizeTopology_ = true;
  }

  double StandardParticleSwarmOptimisation2011::getAcceleration() noexcept {
    return std::uniform_real_distribution<double>(0.0, 1.0)(Rng::generator);
  }

  arma::Col<double> StandardParticleSwarmOptimisation2011::getVelocity() noexcept {
    return arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::generator);
  }

  arma::Mat<arma::uword> StandardParticleSwarmOptimisation2011::getNeighbourhoodTopology() noexcept {
    arma::Mat<arma::uword> topology = (arma::randu<arma::Mat<double>>(populationSize_, populationSize_) <= neighbourhoodProbability_);
    topology.diag() += 1.0;

    return topology;
  }

  void StandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(
      const double& neighbourhoodProbability) noexcept {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void StandardParticleSwarmOptimisation2011::setMaximalAcceleration(
      const double& maximalAcceleration) noexcept {
    maximalAcceleration_ = maximalAcceleration;
  }

  void StandardParticleSwarmOptimisation2011::setMaximalLocalAttraction(
      const double& maximalLocalAttraction) noexcept {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  void StandardParticleSwarmOptimisation2011::setMaximalGlobalAttraction(
      const double& maximalGlobalAttraction) noexcept {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  void StandardParticleSwarmOptimisation2011::setMaximalSwarmConvergence(
      const double& maximalSwarmConvergence) noexcept {
    maximalSwarmConvergence_ = maximalSwarmConvergence;
  }

  std::string StandardParticleSwarmOptimisation2011::to_string() const noexcept {
    return "StandardParticleSwarmOptimisation2011";
  }
}
