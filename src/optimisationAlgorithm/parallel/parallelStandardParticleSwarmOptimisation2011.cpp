#include <hop_bits/optimisationAlgorithm/parallel/parallelStandardParticleSwarmOptimisation2011.hpp>

// C++ Standard Library
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>
#include <iostream>

// MPI
#include <mpi.h>

// HOP
#include <hop_bits/helper/random.hpp>
#include <hop_bits/helper/rng.hpp>

namespace hop {
  ParallelStandardParticleSwarmOptimisation2011::ParallelStandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem> optimisationProblem,
      const unsigned int& localPopulationSize)
    : ParallelOptimisationAlgorithm(optimisationProblem),
      localPopulationSize_(localPopulationSize) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(localPopulationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
    setCommunicationSteps(1);
  }

  void ParallelStandardParticleSwarmOptimisation2011::parallelOptimiseImplementation() {
    arma::Mat<double> localParticles = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), localPopulationSize_);
    localParticles.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    localParticles.each_col() += optimisationProblem_->getLowerBounds();

    arma::Mat<double> localVelocities = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), localPopulationSize_);
    localVelocities.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    localVelocities.each_col() += optimisationProblem_->getLowerBounds();
    localVelocities -= localParticles;

    arma::Mat<double> localBestSolutions(optimisationProblem_->getNumberOfDimensions(), localPopulationSize_ * numberOfNodes_);
    localBestSolutions.cols(rank_ * localPopulationSize_, (rank_ + 1) * localPopulationSize_ - 1) = localParticles;

    arma::Col<double> localBestObjectiveValues(localPopulationSize_ * numberOfNodes_);
    for (std::size_t n = 0; n < localPopulationSize_; ++n) {
      ++numberOfIterations_;
      arma::Col<double> localBestSolution = localBestSolutions.col(rank_ * localPopulationSize_ + n);
      double localBestObjectiveValue = optimisationProblem_->getObjectiveValue(localBestSolution) + optimisationProblem_->getSoftConstraintsValue(localBestSolution);
      localBestObjectiveValues.at(rank_ * localPopulationSize_ + n) = localBestObjectiveValue;

      if (isFinished() || isTerminated()) {
        break;
      }
    }

    arma::Mat<double> localBestSolutionsSend = localBestSolutions.cols(rank_ * localPopulationSize_, (rank_ + 1) * localPopulationSize_ - 1);
    MPI_Allgather(
          localBestSolutionsSend.memptr(),
          localPopulationSize_ * optimisationProblem_->getNumberOfDimensions(),
          MPI_DOUBLE,
          localBestSolutions.memptr(),
          localPopulationSize_ * optimisationProblem_->getNumberOfDimensions(),
          MPI_DOUBLE,
          MPI_COMM_WORLD);

    arma::Col<double> localBestObjectiveValuesSend = localBestObjectiveValues.subvec(rank_ * localPopulationSize_, (rank_ + 1) * localPopulationSize_ - 1);
    MPI_Allgather(
          localBestObjectiveValuesSend.memptr(),
          localPopulationSize_,
          MPI_DOUBLE,
          localBestObjectiveValues.memptr(),
          localPopulationSize_,
          MPI_DOUBLE,
          MPI_COMM_WORLD);

    arma::uword bestSolutionIndex;
    bestObjectiveValue_ = localBestObjectiveValues.min(bestSolutionIndex);
    bestSolution_ = localBestSolutions.col(bestSolutionIndex);

    bool randomizeTopology = true;

    arma::Mat<arma::uword> topology(localPopulationSize_ * numberOfNodes_, localPopulationSize_);
    while(!isFinished() && !isTerminated()) {
      for (unsigned int k = 0; k < communicationSteps_; ++k) {
        if (randomizeTopology) {
            topology = (arma::randu<arma::Mat<double>>(localPopulationSize_ * numberOfNodes_, localPopulationSize_) <= neighbourhoodProbability_);
            topology.diag(-rank_ * localPopulationSize_) += 1.0;

            randomizeTopology = false;
        }

        if (rank_ == 2) {
//          std::cout << "topology: " << topology << std::endl;
        }

        arma::Col<arma::uword> permutation = getRandomPermutation(localPopulationSize_);
        for (std::size_t n = 0; n < localPopulationSize_; ++n) {
          ++numberOfIterations_;

          std::size_t nn = permutation.at(n);
          arma::Col<double> particle = localParticles.col(nn);

          arma::uword neighbourhoodBestParticleIndex;
          arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(topology.col(nn));
          static_cast<arma::Col<double>>(localBestObjectiveValues.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

          neighbourhoodBestParticleIndex = neighbourhoodParticlesIndecies.at(neighbourhoodBestParticleIndex);

          arma::Col<double> attractionCenter;
          if (neighbourhoodBestParticleIndex == rank_ * localPopulationSize_ + nn) {
            attractionCenter = (localAttraction_ * (localBestSolutions.col(rank_ * localPopulationSize_ + nn) - particle)) / 2.0;
          } else {
            attractionCenter = (localAttraction_ * (localBestSolutions.col(rank_ * localPopulationSize_ + nn) - particle) + globalAttraction_ * (localBestSolutions.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
          }

          arma::Col<double> velocityCandidate = acceleration_ * localVelocities.col(nn) + arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Rng::generator) * arma::norm(attractionCenter) + attractionCenter;
          arma::Col<double> solutionCandidate = particle + velocityCandidate;

          arma::Col<arma::uword> belowLowerBound = arma::find(solutionCandidate < optimisationProblem_->getLowerBounds());
          arma::Col<arma::uword> aboveUpperBound = arma::find(solutionCandidate > optimisationProblem_->getUpperBounds());

          velocityCandidate.elem(belowLowerBound) *= -0.5;
          velocityCandidate.elem(aboveUpperBound) *= -0.5;

          solutionCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
          solutionCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

          localVelocities.col(nn) = velocityCandidate;
          localParticles.col(nn) = solutionCandidate;

          if (rank_ == 2) {
//            std::cout << "localVelocities: " << localVelocities << std::endl;
          }

          double objectiveValue = optimisationProblem_->getObjectiveValue(solutionCandidate) + optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

          if (objectiveValue < localBestObjectiveValues.at(nn)) {
            localBestObjectiveValues.at(rank_ * localPopulationSize_ + nn) = objectiveValue;
            localBestSolutions.col(rank_ * localPopulationSize_ + nn) = solutionCandidate;
          }

          if (objectiveValue < bestObjectiveValue_) {
            bestObjectiveValue_ = objectiveValue;
            bestSolution_ = solutionCandidate;
          } else {
            randomizeTopology = true;
          }

          if (isFinished() || isTerminated()) {
            break;
          }
        }

        if (isFinished() || isTerminated()) {
          break;
        }
      }

      arma::Mat<double> localBestSolutionsSend = localBestSolutions.cols(rank_ * localPopulationSize_, (rank_ + 1) * localPopulationSize_ - 1);
      MPI_Allgather(
            localBestSolutionsSend.memptr(),
            localPopulationSize_ * optimisationProblem_->getNumberOfDimensions(),
            MPI_DOUBLE,
            localBestSolutions.memptr(),
            localPopulationSize_ * optimisationProblem_->getNumberOfDimensions(),
            MPI_DOUBLE,
            MPI_COMM_WORLD);

      arma::Col<double> localBestObjectiveValuesSend = localBestObjectiveValues.subvec(rank_ * localPopulationSize_, (rank_ + 1) * localPopulationSize_ - 1);
      MPI_Allgather(
            localBestObjectiveValuesSend.memptr(),
            localPopulationSize_,
            MPI_DOUBLE,
            localBestObjectiveValues.memptr(),
            localPopulationSize_,
            MPI_DOUBLE,
            MPI_COMM_WORLD);

      arma::uword bestSolutionIndex;
      double bestObjectiveValue = localBestObjectiveValues.min(bestSolutionIndex);
      if (bestObjectiveValue < bestObjectiveValue_) {
        bestObjectiveValue_ = bestObjectiveValue;
        bestSolution_ = localBestSolutions.col(bestSolutionIndex);
      } else {
        randomizeTopology = true;
      }
    }
  }

  void ParallelStandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(
      const double& neighbourhoodProbability) {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setAcceleration(
      const double& acceleration) {
    acceleration_ = acceleration;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setLocalAttraction(
      const double& localAttraction) {
    localAttraction_ = localAttraction;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setGlobalAttraction(
      const double& globalAttraction) {
    globalAttraction_ = globalAttraction;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setCommunicationSteps(
      const unsigned int& communicationSteps) {
    communicationSteps_ = communicationSteps;
  }

  std::string ParallelStandardParticleSwarmOptimisation2011::to_string() const {
    return "ParallelStandardParticleSwarmOptimisation2011";
  }
}
