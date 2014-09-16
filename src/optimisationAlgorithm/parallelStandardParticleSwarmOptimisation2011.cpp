#include <hop_bits/optimisationAlgorithm/parallelStandardParticleSwarmOptimisation2011.hpp>

#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

#include <hop_bits/helper/random.hpp>

namespace hop {
  ParallelStandardParticleSwarmOptimisation2011::ParallelStandardParticleSwarmOptimisation2011(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize)
    : OptimisationAlgorithm(optimisationProblem),
      populationSize_(populationSize),
      randomizeTopology_(populationSize_),
      topology_(populationSize_, populationSize_),
      globalBestObjectiveValues_(populationSize_) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(populationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
    setCommunicationSteps(1);
  }

  bool ParallelStandardParticleSwarmOptimisation2011::optimiseImplementation() {
    particles_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    particles_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    particles_.each_col() += optimisationProblem_->getLowerBounds();

    velocities_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    velocities_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    velocities_.each_col() += optimisationProblem_->getLowerBounds();
    velocities_ -= particles_;

    localBestParticles_ = {particles_, particles_};

    arma::Row<double> localBestObjectiveValues(populationSize_);
    ++numberOfIterations_;
    for(std::size_t n = 0; n < populationSize_; ++n) {
      localBestObjectiveValues.at(n) = optimisationProblem_->getObjectiveValue(localBestParticles_.at(0).col(n)) + optimisationProblem_->getSoftConstraintsValue(localBestParticles_.at(0).col(n));
    }
    localBestObjectiveValues_ = {localBestObjectiveValues, localBestObjectiveValues};

    arma::uword bestParticleIndex;
    bestObjectiveValue_ = localBestObjectiveValues_.at(0).min(bestParticleIndex);
    bestSolution_ = localBestParticles_.at(0).col(bestParticleIndex);

    globalBestObjectiveValues_.fill(bestObjectiveValue_);

    std::fill(randomizeTopology_.begin(), randomizeTopology_.end(), true);

    bool finished = false;
    while(true) {
      for(std::size_t k = 0; k < communicationSteps_; ++k) {
        ++numberOfIterations_;
        for(std::size_t n = 0; n < populationSize_; ++n) {

          if (randomizeTopology_.at(n)) {
              topology_.col(n) = (arma::randu<arma::Col<arma::uword>>(populationSize_) <= neighbourhoodProbability_);
              topology_.at(n, n) = 0.0;

              randomizeTopology_.at(n) = false;
          }

          arma::Col<double> particle = particles_.col(n);

          arma::uword neighbourhoodBestParticleIndex;
          arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(topology_.col(n));
          double neighbourhoodBestObjectiveValue = std::numeric_limits<double>::infinity();
          if(!neighbourhoodParticlesIndecies.is_empty()) {
            neighbourhoodBestObjectiveValue = static_cast<arma::Col<double>>(localBestObjectiveValues_.at(0).elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);
          }

          arma::Col<double> neighbourhoodBestParticle;
          if(neighbourhoodBestObjectiveValue < localBestObjectiveValues_.at(1).at(n)) {
            neighbourhoodBestParticle = localBestParticles_.at(0).col(neighbourhoodParticlesIndecies.at(neighbourhoodBestParticleIndex));
          } else {
            neighbourhoodBestParticle = n;
            neighbourhoodBestParticle = localBestParticles_.at(1).col(n);
          }

          arma::Col<double> attractionCenter;
          if (neighbourhoodBestParticleIndex == n) {
            attractionCenter = (localAttraction_ * (localBestParticles_.at(1).col(n) - particle)) / 2.0;
          } else {
            attractionCenter = (localAttraction_ * (localBestParticles_.at(1).col(n) - particle) + globalAttraction_ * (neighbourhoodBestParticle - particle)) / 3.0;
          }

          arma::Col<double> randomParticle = arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Random::Rng) * arma::norm(attractionCenter) + attractionCenter;

          arma::Col<double> velocityCandidate = acceleration_ * velocities_.col(n) + randomParticle;
          arma::Col<double> particleCandidate = particle + velocityCandidate;

          arma::Col<arma::uword> belowLowerBound = arma::find(particleCandidate < optimisationProblem_->getLowerBounds());
          arma::Col<arma::uword> aboveUpperBound = arma::find(particleCandidate > optimisationProblem_->getUpperBounds());

          velocityCandidate.elem(belowLowerBound) *= -0.5;
          velocityCandidate.elem(aboveUpperBound) *= -0.5;

          particleCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
          particleCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

          velocities_.col(n) = velocityCandidate;
          particles_.col(n) = particleCandidate;


          double objectiveValue = optimisationProblem_->getObjectiveValue(particleCandidate) + optimisationProblem_->getSoftConstraintsValue(particleCandidate);

          if (objectiveValue < localBestObjectiveValues_.at(1).at(n)) {
            localBestObjectiveValues_.at(1).at(n) = objectiveValue;
            localBestParticles_.at(1).col(n) = particleCandidate;
          }

          if (objectiveValue < globalBestObjectiveValues_.at(n)) {
            globalBestObjectiveValues_.at(n) = objectiveValue;
          } else {
            randomizeTopology_.at(n) = true;
          }

          if(globalBestObjectiveValues_.at(n) <= optimisationProblem_->getAcceptableObjectiveValue() || isTerminated()) {
            finished = true;
            break;
          }
        }

        if(finished || isTerminated()) {
          break;
        }
      }

      localBestObjectiveValues_.at(0) = localBestObjectiveValues_.at(1);
      localBestParticles_.at(0) = localBestParticles_.at(1);

      arma::uword bestParticleIndex;
      bestObjectiveValue_ = globalBestObjectiveValues_.min(bestParticleIndex);
      bestSolution_ = localBestParticles_.at(0).col(bestParticleIndex);

      globalBestObjectiveValues_.fill(bestObjectiveValue_);

      if(finished || isTerminated()) {
        return finished;
      }
    }
  }

  void ParallelStandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(const double& neighbourhoodProbability) {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setAcceleration(const double& acceleration) {
    acceleration_ = acceleration;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setLocalAttraction(const double& localAttraction) {
    localAttraction_ = localAttraction;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setGlobalAttraction(const double& globalAttraction) {
    globalAttraction_ = globalAttraction;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setCommunicationSteps(const unsigned int& communicationSteps) {
    communicationSteps_ = communicationSteps;
  }
}
