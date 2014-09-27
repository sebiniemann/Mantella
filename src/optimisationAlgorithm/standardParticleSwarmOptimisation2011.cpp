#include <hop_bits/optimisationAlgorithm/standardParticleSwarmOptimisation2011.hpp>

#include <cmath>
#include <random>

#include <hop_bits/helper/random.hpp>

namespace hop {
  StandardParticleSwarmOptimisation2011::StandardParticleSwarmOptimisation2011(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize)
    : OptimisationAlgorithm(optimisationProblem),
      populationSize_(populationSize),
      localBestObjectiveValues_(populationSize_) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(populationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
  }

  void StandardParticleSwarmOptimisation2011::optimiseImplementation() {
    particles_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    particles_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    particles_.each_col() += optimisationProblem_->getLowerBounds();

    velocities_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    velocities_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    velocities_.each_col() += optimisationProblem_->getLowerBounds();
    velocities_ -= particles_;

    localBestParticles_ = particles_;

    for(std::size_t n = 0; n < populationSize_; ++n) {
      ++numberOfIterations_;
      localBestObjectiveValues_.at(n) = optimisationProblem_->getObjectiveValue(localBestParticles_.col(n)) + optimisationProblem_->getSoftConstraintsValue(localBestParticles_.col(n));
    }

    arma::uword bestParticleIndex;
    bestObjectiveValue_ = localBestObjectiveValues_.min(bestParticleIndex);
    bestSolution_ = localBestParticles_.col(bestParticleIndex);

    randomizeTopology_ = true;

    while(true) {
      if (randomizeTopology_) {
          topology_ = (arma::randu<arma::Mat<arma::uword>>(populationSize_, populationSize_) <= neighbourhoodProbability_);
          topology_.diag() += 1.0;

          randomizeTopology_ = false;
      }

      for(std::size_t n = 0; n < populationSize_; ++n) {
        ++numberOfIterations_;

        arma::Col<double> particle = particles_.col(n);

        arma::uword neighbourhoodBestParticleIndex;
        arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(topology_.col(n));
        static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

        neighbourhoodBestParticleIndex = neighbourhoodParticlesIndecies.at(neighbourhoodBestParticleIndex);

        arma::Col<double> attractionCenter;
        if (neighbourhoodBestParticleIndex == n) {
          attractionCenter = (localAttraction_ * (localBestParticles_.col(n) - particle)) / 2.0;
        } else {
          attractionCenter = (localAttraction_ * (localBestParticles_.col(n) - particle) + globalAttraction_ * (localBestParticles_.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
        }

        arma::Col<double> randomParticle = arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Random::Rng) * arma::norm(attractionCenter) + attractionCenter;

        arma::Col<double> velocityCandidate = acceleration_ * velocities_.col(n) + randomParticle;
        arma::Col<double> solutionCandidate = particle + velocityCandidate;

        arma::Col<arma::uword> belowLowerBound = arma::find(solutionCandidate < optimisationProblem_->getLowerBounds());
        arma::Col<arma::uword> aboveUpperBound = arma::find(solutionCandidate > optimisationProblem_->getUpperBounds());

        velocityCandidate.elem(belowLowerBound) *= -0.5;
        velocityCandidate.elem(aboveUpperBound) *= -0.5;

        solutionCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
        solutionCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

        velocities_.col(n) = velocityCandidate;
        particles_.col(n) = solutionCandidate;

        double objectiveValue = optimisationProblem_->getObjectiveValue(solutionCandidate) + optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

        if (objectiveValue < localBestObjectiveValues_.at(n)) {
          localBestObjectiveValues_.at(n) = objectiveValue;
          localBestParticles_.col(n) = solutionCandidate;
        }

        if (objectiveValue < bestObjectiveValue_) {
          bestObjectiveValue_ = objectiveValue;
          bestSolution_ = solutionCandidate;
        } else {
          randomizeTopology_ = true;
        }

        if(isFinished() || isTerminated()) {
          return;
        }
      }
    }
  }

  void StandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(const double& neighbourhoodProbability) {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void StandardParticleSwarmOptimisation2011::setAcceleration(const double& acceleration) {
    acceleration_ = acceleration;
  }

  void StandardParticleSwarmOptimisation2011::setLocalAttraction(const double& localAttraction) {
    localAttraction_ = localAttraction;
  }

  void StandardParticleSwarmOptimisation2011::setGlobalAttraction(const double& globalAttraction) {
    globalAttraction_ = globalAttraction;
  }
}
