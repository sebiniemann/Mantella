#include <hop_bits/optimisationAlgorithm/standardParticleSwarmOptimisation2011.hpp>

// C++ Standard Library
#include <cmath>
#include <random>

// HOP
#include <hop_bits/helper/random.hpp>
#include <hop_bits/helper/rng.hpp>

namespace hop {
  StandardParticleSwarmOptimisation2011::StandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const unsigned int& populationSize)
    : OptimisationAlgorithm(optimisationProblem),
      populationSize_(populationSize),
      localBestObjectiveValues_(populationSize_),
      randomizeTopology_(true) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(populationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
    setMaximalSwarmConvergence(0.05); // TODO Check value within the paper
  }

  void StandardParticleSwarmOptimisation2011::optimiseImplementation() {
    initialiseSwarm();

    while(!isFinished() && !isTerminated()) {
      if (randomizeTopology_) {
          topology_ = (arma::randu<arma::Mat<double>>(populationSize_, populationSize_) <= neighbourhoodProbability_);
          topology_.diag() += 1.0;

          randomizeTopology_ = false;
      }

      arma::Col<arma::uword> permutation = getRandomPermutation(populationSize_);
      for (std::size_t n = 0; n < populationSize_; ++n) {
        ++numberOfIterations_;

        std::size_t k = permutation.at(n);
        arma::Col<double> particle = particles_.col(k);

        arma::uword neighbourhoodBestParticleIndex;
        arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(topology_.col(k));
        static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

        neighbourhoodBestParticleIndex = neighbourhoodParticlesIndecies.at(neighbourhoodBestParticleIndex);

        arma::Col<double> attractionCenter;
        if (neighbourhoodBestParticleIndex == k) {
          attractionCenter = (localAttraction_ * (localBestSolutions_.col(k) - particle)) / 2.0;
        } else {
          attractionCenter = (localAttraction_ * (localBestSolutions_.col(k) - particle) + globalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
        }

        arma::Col<double> randomParticle = arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Rng::generator) * arma::norm(attractionCenter) + attractionCenter;

        arma::Col<double> velocityCandidate = acceleration_ * velocities_.col(k) + randomParticle;
        arma::Col<double> solutionCandidate = particle + velocityCandidate;

        arma::Col<arma::uword> belowLowerBound = arma::find(solutionCandidate < optimisationProblem_->getLowerBounds());
        arma::Col<arma::uword> aboveUpperBound = arma::find(solutionCandidate > optimisationProblem_->getUpperBounds());

        velocityCandidate.elem(belowLowerBound) *= -0.5;
        velocityCandidate.elem(aboveUpperBound) *= -0.5;

        solutionCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
        solutionCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

        velocities_.col(k) = velocityCandidate;
        particles_.col(k) = solutionCandidate;

        double objectiveValue = optimisationProblem_->getObjectiveValue(solutionCandidate) + optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

        if (objectiveValue < localBestObjectiveValues_.at(k)) {
          localBestObjectiveValues_.at(k) = objectiveValue;
          localBestSolutions_.col(k) = solutionCandidate;
        }

        if (objectiveValue < bestObjectiveValue_) {
          bestObjectiveValue_ = objectiveValue;
          bestSolution_ = solutionCandidate;
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

  void StandardParticleSwarmOptimisation2011::initialiseSwarm() {
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
        bestSolution_ = localBestSolution;
        bestObjectiveValue_ = localBestObjectiveValue;
      }

      if (isFinished() || isTerminated()) {
        break;
      }
    }

    randomizeTopology_ = true;
  }

  void StandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(
      const double& neighbourhoodProbability) {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void StandardParticleSwarmOptimisation2011::setAcceleration(
      const double& acceleration) {
    acceleration_ = acceleration;
  }

  void StandardParticleSwarmOptimisation2011::setLocalAttraction(
      const double& localAttraction) {
    localAttraction_ = localAttraction;
  }

  void StandardParticleSwarmOptimisation2011::setGlobalAttraction(
      const double& globalAttraction) {
    globalAttraction_ = globalAttraction;
  }

  void StandardParticleSwarmOptimisation2011::setMaximalSwarmConvergence(
      const double& maximalSwarmConvergence) {
    maximalSwarmConvergence_ = maximalSwarmConvergence;
  }

  std::string StandardParticleSwarmOptimisation2011::to_string() const {
    return "StandardParticleSwarmOptimisation2011";
  }
}
