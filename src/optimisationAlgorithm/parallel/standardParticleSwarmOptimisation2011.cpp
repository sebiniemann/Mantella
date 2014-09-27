#include <hop_bits/optimisationAlgorithm/parallel/standardParticleSwarmOptimisation2011.hpp>

#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

#include <mpi.h>

#include <hop_bits/helper/random.hpp>

namespace hop {
  ParallelStandardParticleSwarmOptimisation2011::ParallelStandardParticleSwarmOptimisation2011(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& localPopulationSize)
    : ParallelOptimisationAlgorithm(optimisationProblem),
      localPopulationSize_(localPopulationSize),
      localBestObjectiveValues_(localPopulationSize_),
      topology_(localPopulationSize_, localPopulationSize_) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(localPopulationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
    setCommunicationSteps(1);
  }

  void ParallelStandardParticleSwarmOptimisation2011::parallelOptimiseImplementation() {
    localParticles_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), localPopulationSize_);
    localParticles_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    localParticles_.each_col() += optimisationProblem_->getLowerBounds();

    localVelocities_ = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), localPopulationSize_);
    localVelocities_.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    localVelocities_.each_col() += optimisationProblem_->getLowerBounds();
    localVelocities_ -= localParticles_;

    localBestSolutions_ = localParticles_;

    ++numberOfIterations_;
    for(std::size_t n = 0; n < localPopulationSize_; ++n) {
      arma::Col<double> localBestSolution = localBestSolutions_.col(n);
      double localBestObjectiveValue = optimisationProblem_->getObjectiveValue(localBestSolution) + optimisationProblem_->getSoftConstraintsValue(localBestSolution);

      if(localBestObjectiveValue < bestObjectiveValue_) {
        bestSolution_ = localBestSolution;
        bestObjectiveValue_ = localBestObjectiveValue;
      }

      if(isFinished() || isTerminated()) {
        break;
      }
    }

    struct {
        double value;
        int rank;
    } bestObjectiveValueInput, bestObjectiveValueOutput;

    bestObjectiveValueInput.value = bestObjectiveValue_;
    bestObjectiveValueInput.rank = rank_;

    MPI_Allreduce(&bestObjectiveValueInput, &bestObjectiveValueOutput, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
    MPI_Bcast(bestSolution_.memptr(), bestSolution_.n_elem, MPI_DOUBLE, bestObjectiveValueOutput.rank, MPI_COMM_WORLD);
    bestObjectiveValue_ = bestObjectiveValueOutput.value;

    if(isFinished() || isTerminated()) {
      return;
    }

    do {
      for(unsigned int k = 0; k < communicationSteps_; ++k) {
        if (randomizeTopology_) {
            topology_ = (arma::randu<arma::Mat<arma::uword>>(localPopulationSize_, localPopulationSize_) <= neighbourhoodProbability_);
            topology_.diag() += 1.0;

            randomizeTopology_ = false;
        }

        for(std::size_t n = 0; n < localPopulationSize_; ++n) {
          ++numberOfIterations_;

          arma::Col<double> particle = localParticles_.col(n);

          arma::uword neighbourhoodBestParticleIndex;
          arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(topology_.col(n));
          static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

          neighbourhoodBestParticleIndex = neighbourhoodParticlesIndecies.at(neighbourhoodBestParticleIndex);

          arma::Col<double> attractionCenter;
          if (neighbourhoodBestParticleIndex == n) {
            attractionCenter = (localAttraction_ * (localBestSolutions_.col(n) - particle)) / 2.0;
          } else {
            attractionCenter = (localAttraction_ * (localBestSolutions_.col(n) - particle) + globalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
          }

          arma::Col<double> velocityCandidate = acceleration_ * localVelocities_.col(n) + arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Random::Rng) * arma::norm(attractionCenter) + attractionCenter;
          arma::Col<double> solutionCandidate = particle + velocityCandidate;

          arma::Col<arma::uword> belowLowerBound = arma::find(solutionCandidate < optimisationProblem_->getLowerBounds());
          arma::Col<arma::uword> aboveUpperBound = arma::find(solutionCandidate > optimisationProblem_->getUpperBounds());

          velocityCandidate.elem(belowLowerBound) *= -0.5;
          velocityCandidate.elem(aboveUpperBound) *= -0.5;

          solutionCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
          solutionCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

          localVelocities_.col(n) = velocityCandidate;
          localParticles_.col(n) = solutionCandidate;

          double objectiveValue = optimisationProblem_->getObjectiveValue(solutionCandidate) + optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

          if (objectiveValue < localBestObjectiveValues_.at(n)) {
            localBestObjectiveValues_.at(n) = objectiveValue;
            localBestSolutions_.col(n) = solutionCandidate;
          }

          if (objectiveValue < bestObjectiveValue_) {
            bestObjectiveValue_ = objectiveValue;
            bestSolution_ = solutionCandidate;
          } else {
            randomizeTopology_ = true;
          }

          if(isFinished() || isTerminated()) {
            break;
          }
        }

        if(isFinished() || isTerminated()) {
          break;
        }
      };

      bestObjectiveValueInput.value = bestObjectiveValue_;

      MPI_Allreduce(&bestObjectiveValueInput, &bestObjectiveValueOutput, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);
      MPI_Bcast(bestSolution_.memptr(), bestSolution_.n_elem, MPI_DOUBLE, bestObjectiveValueOutput.rank, MPI_COMM_WORLD);
      bestObjectiveValue_ = bestObjectiveValueOutput.value;

      if(isFinished() || isTerminated()) {
        return;
      }
    } while(true);
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
