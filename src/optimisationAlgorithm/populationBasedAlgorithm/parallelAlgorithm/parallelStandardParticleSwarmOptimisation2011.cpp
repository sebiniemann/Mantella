#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm/parallelAlgorithm/parallelStandardParticleSwarmOptimisation2011.hpp>

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
      const unsigned int& populationSize) noexcept
    : ParallelAlgorithm(optimisationProblem, populationSize) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(populationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
    setCommunicationSteps(1);
  }

  void ParallelStandardParticleSwarmOptimisation2011::parallelOptimiseImplementation() noexcept {
    arma::Mat<double> localParticles = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    localParticles.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    localParticles.each_col() += optimisationProblem_->getLowerBounds();

    arma::Mat<double> localVelocities = arma::randu<arma::Mat<double>>(optimisationProblem_->getNumberOfDimensions(), populationSize_);
    localVelocities.each_col() %= optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds();
    localVelocities.each_col() += optimisationProblem_->getLowerBounds();
    localVelocities -= localParticles;

    arma::Mat<double> localBestSolutions(optimisationProblem_->getNumberOfDimensions(), populationSize_ * numberOfNodes_);
    localBestSolutions.cols(rank_ * populationSize_, (rank_ + 1) * populationSize_ - 1) = localParticles;

    arma::Col<double> localBestObjectiveValues(populationSize_ * numberOfNodes_);
    for (std::size_t n = 0; n < populationSize_; ++n) {
      ++numberOfIterations_;
      arma::Col<double> localBestSolution = localBestSolutions.col(rank_ * populationSize_ + n);
      double localBestObjectiveValue = optimisationProblem_->getObjectiveValue(localBestSolution) + optimisationProblem_->getSoftConstraintsValue(localBestSolution);
      localBestObjectiveValues.at(rank_ * populationSize_ + n) = localBestObjectiveValue;

      if (isFinished() || isTerminated()) {
        break;
      }
    }

    arma::Mat<double> localBestSolutionsSend = localBestSolutions.cols(rank_ * populationSize_, (rank_ + 1) * populationSize_ - 1);
    MPI_Allgather(
          localBestSolutionsSend.memptr(),
          populationSize_ * optimisationProblem_->getNumberOfDimensions(),
          MPI_DOUBLE,
          localBestSolutions.memptr(),
          populationSize_ * optimisationProblem_->getNumberOfDimensions(),
          MPI_DOUBLE,
          MPI_COMM_WORLD);

    arma::Col<double> localBestObjectiveValuesSend = localBestObjectiveValues.subvec(rank_ * populationSize_, (rank_ + 1) * populationSize_ - 1);
    MPI_Allgather(
          localBestObjectiveValuesSend.memptr(),
          populationSize_,
          MPI_DOUBLE,
          localBestObjectiveValues.memptr(),
          populationSize_,
          MPI_DOUBLE,
          MPI_COMM_WORLD);

    arma::uword bestSolutionIndex;
    bestObjectiveValue_ = localBestObjectiveValues.min(bestSolutionIndex);
    bestParameter_ = localBestSolutions.col(bestSolutionIndex);

    bool randomizeTopology = true;

    arma::Mat<arma::uword> topology(populationSize_ * numberOfNodes_, populationSize_);
    while(!isFinished() && !isTerminated()) {
      for (unsigned int k = 0; k < communicationSteps_; ++k) {
        if (randomizeTopology) {
            topology = (arma::randu<arma::Mat<double>>(populationSize_ * numberOfNodes_, populationSize_) <= neighbourhoodProbability_);
            topology.diag(-rank_ * populationSize_) += 1.0;

            randomizeTopology = false;
        }

        if (rank_ == 2) {
//          std::cout << "topology: " << topology << std::endl;
        }

        arma::Col<arma::uword> permutation = getRandomPermutation(populationSize_);
        for (std::size_t n = 0; n < populationSize_; ++n) {
          ++numberOfIterations_;

          std::size_t k = permutation.at(n);
          arma::Col<double> particle = localParticles.col(k);

          arma::uword neighbourhoodBestParticleIndex;
          arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(topology.col(k));
          static_cast<arma::Col<double>>(localBestObjectiveValues.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

          neighbourhoodBestParticleIndex = neighbourhoodParticlesIndecies.at(neighbourhoodBestParticleIndex);

          arma::Col<double> attractionCenter;
          if (neighbourhoodBestParticleIndex == rank_ * populationSize_ + k) {
            attractionCenter = (localAttraction_ * (localBestSolutions.col(rank_ * populationSize_ + k) - particle)) / 2.0;
          } else {
            attractionCenter = (localAttraction_ * (localBestSolutions.col(rank_ * populationSize_ + k) - particle) + globalAttraction_ * (localBestSolutions.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
          }

          arma::Col<double> velocityCandidate = acceleration_ * localVelocities.col(k) + arma::normalise(arma::randn<arma::Col<double>>(optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0, 1)(Rng::generator) * arma::norm(attractionCenter) + attractionCenter;
          arma::Col<double> solutionCandidate = particle + velocityCandidate;

          arma::Col<arma::uword> belowLowerBound = arma::find(solutionCandidate < optimisationProblem_->getLowerBounds());
          arma::Col<arma::uword> aboveUpperBound = arma::find(solutionCandidate > optimisationProblem_->getUpperBounds());

          velocityCandidate.elem(belowLowerBound) *= -0.5;
          velocityCandidate.elem(aboveUpperBound) *= -0.5;

          solutionCandidate.elem(belowLowerBound) = optimisationProblem_->getLowerBounds().elem(belowLowerBound);
          solutionCandidate.elem(aboveUpperBound) = optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

          localVelocities.col(k) = velocityCandidate;
          localParticles.col(k) = solutionCandidate;

          if (rank_ == 2) {
//            std::cout << "localVelocities: " << localVelocities << std::endl;
          }

          double objectiveValue = optimisationProblem_->getObjectiveValue(solutionCandidate) + optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

          if (objectiveValue < localBestObjectiveValues.at(k)) {
            localBestObjectiveValues.at(rank_ * populationSize_ + k) = objectiveValue;
            localBestSolutions.col(rank_ * populationSize_ + k) = solutionCandidate;
          }

          if (objectiveValue < bestObjectiveValue_) {
            bestObjectiveValue_ = objectiveValue;
            bestParameter_ = solutionCandidate;
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

      arma::Mat<double> localBestSolutionsSend = localBestSolutions.cols(rank_ * populationSize_, (rank_ + 1) * populationSize_ - 1);
      MPI_Allgather(
            localBestSolutionsSend.memptr(),
            populationSize_ * optimisationProblem_->getNumberOfDimensions(),
            MPI_DOUBLE,
            localBestSolutions.memptr(),
            populationSize_ * optimisationProblem_->getNumberOfDimensions(),
            MPI_DOUBLE,
            MPI_COMM_WORLD);

      arma::Col<double> localBestObjectiveValuesSend = localBestObjectiveValues.subvec(rank_ * populationSize_, (rank_ + 1) * populationSize_ - 1);
      MPI_Allgather(
            localBestObjectiveValuesSend.memptr(),
            populationSize_,
            MPI_DOUBLE,
            localBestObjectiveValues.memptr(),
            populationSize_,
            MPI_DOUBLE,
            MPI_COMM_WORLD);

      arma::uword bestSolutionIndex;
      double bestObjectiveValue = localBestObjectiveValues.min(bestSolutionIndex);
      if (bestObjectiveValue < bestObjectiveValue_) {
        bestObjectiveValue_ = bestObjectiveValue;
        bestParameter_ = localBestSolutions.col(bestSolutionIndex);
      } else {
        randomizeTopology = true;
      }
    }
  }

  void ParallelStandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(
      const double& neighbourhoodProbability) noexcept {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setAcceleration(
      const double& acceleration) noexcept {
    acceleration_ = acceleration;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setLocalAttraction(
      const double& localAttraction) noexcept {
    localAttraction_ = localAttraction;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setGlobalAttraction(
      const double& globalAttraction) noexcept {
    globalAttraction_ = globalAttraction;
  }

  void ParallelStandardParticleSwarmOptimisation2011::setCommunicationSteps(
      const unsigned int& communicationSteps) noexcept {
    communicationSteps_ = communicationSteps;
  }

  std::string ParallelStandardParticleSwarmOptimisation2011::to_string() const noexcept {
    return "ParallelStandardParticleSwarmOptimisation2011";
  }
}
