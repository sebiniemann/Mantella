#include <mantella_bits/optimisationAlgorithm.hpp>

// C++ standard library
#include <cassert>
#include <random>

// Mantella
#include <mantella_bits/config.hpp>
#include <mantella_bits/helper/assert.hpp>
#include <mantella_bits/helper/rng.hpp>
#include <mantella_bits/helper/mpi.hpp>

namespace mant {
  OptimisationAlgorithm::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
      : optimisationProblem_(optimisationProblem),
        numberOfDimensions_(optimisationProblem_->numberOfDimensions_),
        numberOfIterations_(0),
        bestObjectiveValue_(std::numeric_limits<double>::infinity()),
        bestParameter_(numberOfDimensions_) {
    setMaximalNumberOfIterations(1000);

#if defined(SUPPORT_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
#else
    nodeRank_ = 0;
    numberOfNodes_ = 1;
#endif
  }

  void OptimisationAlgorithm::optimise() {
    verify(arma::all(optimisationProblem_->getLowerBounds() <= optimisationProblem_->getUpperBounds()),
        "All upper bounds of the optimisation problem must be greater than or equal to its lower bound.");

#if defined(SUPPORT_MPI)
    std::vector<double> serialisedOptimisationProblem;
    unsigned int serialisedOptimisationProblemSize;

    if (nodeRank_ == 0) {
      serialisedOptimisationProblem = optimisationProblem_->serialise();
      serialisedOptimisationProblemSize = static_cast<unsigned int>(serialisedOptimisationProblem.size());
    }

    MPI_Bcast(&serialisedOptimisationProblemSize, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    if (nodeRank_ != 0) {
      serialisedOptimisationProblem.resize(serialisedOptimisationProblemSize);
    }

    MPI_Bcast(&serialisedOptimisationProblem[0], serialisedOptimisationProblemSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (nodeRank_ != 0) {
      optimisationProblem_->deserialise(serialisedOptimisationProblem);
    }
#endif

    // Resets the results, counters and caches
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestParameter_.fill(std::numeric_limits<double>::quiet_NaN());
    numberOfIterations_ = 0;
    optimisationProblem_->reset();

    optimiseImplementation();

#if defined(SUPPORT_MPI)
    MPI_Datatype MANT_MPI_PARAMETER;
    MPI_Type_contiguous(2 + numberOfDimensions_, MPI_DOUBLE, &MANT_MPI_PARAMETER);
    MPI_Type_commit(&MANT_MPI_PARAMETER);

    MPI_Op MANT_MPI_GET_BEST_PARAMETER;
    MPI_Op_create(&mpiGetBestSample, true, &MANT_MPI_GET_BEST_PARAMETER);

    arma::Col<double> mpiInputParameter(2 + numberOfDimensions_);
    arma::Col<double> mpiOutputParameter(2 + numberOfDimensions_);

    mpiInputParameter(0) = static_cast<double>(numberOfDimensions_);
    mpiInputParameter(1) = bestObjectiveValue_;
    mpiInputParameter.tail(numberOfDimensions_) = bestParameter_;

    MPI_Reduce(mpiInputParameter.memptr(), mpiOutputParameter.memptr(), 1, MANT_MPI_PARAMETER, MANT_MPI_GET_BEST_PARAMETER, 0, MPI_COMM_WORLD);

    MPI_Op_free(&MANT_MPI_GET_BEST_PARAMETER);
    MPI_Type_free(&MANT_MPI_PARAMETER);
#endif
  }

  void OptimisationAlgorithm::setMaximalNumberOfIterations(
      const arma::uword maximalNumberOfIterations) {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  arma::uword OptimisationAlgorithm::getNumberOfIterations() const {
    return numberOfIterations_;
  }

  arma::uword OptimisationAlgorithm::getMaximalNumberOfIterations() const {
    return maximalNumberOfIterations_;
  }

  double OptimisationAlgorithm::getBestObjectiveValue() const {
    return bestObjectiveValue_;
  }

  arma::Col<double> OptimisationAlgorithm::getBestParameter() const {
    return bestParameter_;
  }

  bool OptimisationAlgorithm::isFinished() const {
    return (arma::all(optimisationProblem_->isWithinLowerBounds(bestParameter_)) && arma::all(optimisationProblem_->isWithinUpperBounds(bestParameter_)) && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  bool OptimisationAlgorithm::isTerminated() const {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  std::vector<std::pair<arma::Col<double>, double>> OptimisationAlgorithm::getSamplingHistory() const {
    return samplingHistory_;
  }

  arma::Col<double> OptimisationAlgorithm::getLowerBounds() const {
    return optimisationProblem_->getLowerBounds();
  }

  arma::Col<double> OptimisationAlgorithm::getUpperBounds() const {
    return optimisationProblem_->getUpperBounds();
  }

  arma::Col<arma::uword> OptimisationAlgorithm::isWithinLowerBounds(
      const arma::Col<double>& parameter) const {
    assert(parameter.n_elem == numberOfDimensions_);

    return optimisationProblem_->isWithinLowerBounds(parameter);
  }

  arma::Col<arma::uword> OptimisationAlgorithm::isWithinUpperBounds(
      const arma::Col<double>& parameter) const {
    assert(parameter.n_elem == numberOfDimensions_);

    return optimisationProblem_->isWithinUpperBounds(parameter);
  }

  double OptimisationAlgorithm::getAcceptableObjectiveValue() const {
    return optimisationProblem_->getAcceptableObjectiveValue();
  }

  double OptimisationAlgorithm::getObjectiveValue(
      const arma::Col<double>& parameter) {
    assert(parameter.n_elem == numberOfDimensions_);

    if (recordSamples) {
      const double objectiveValue = optimisationProblem_->getObjectiveValue(parameter);
      samplingHistory_.push_back({parameter, objectiveValue});
      return objectiveValue;
    } else {
      return optimisationProblem_->getObjectiveValue(parameter);
    }
  }

  arma::Col<double> OptimisationAlgorithm::getRandomParameter() const {
    return getLowerBounds() + arma::randu<arma::Col<double>>(numberOfDimensions_) % (getUpperBounds() - getLowerBounds());
  }

  arma::Col<double> OptimisationAlgorithm::getRandomNeighbour(
      const arma::Col<double>& parameter,
      const arma::Col<double>& minimalDistance,
      const arma::Col<double>& maximalDistance) {
    assert(arma::all(maximalDistance > 0));
    assert(arma::all(minimalDistance <= maximalDistance));

    return parameter + arma::normalise(arma::randn<arma::Col<double>>(parameter.n_elem)) % (minimalDistance + arma::randu<arma::Col<double>>(parameter.n_elem) % (maximalDistance - minimalDistance));
  }

  bool OptimisationAlgorithm::updateBestParameter(
      const arma::Col<double>& parameter,
      const double objectiveValue) {
    assert(parameter.n_elem == numberOfDimensions_);

    if (objectiveValue < bestObjectiveValue_) {
      bestParameter_ = parameter;
      bestObjectiveValue_ = objectiveValue;

      return true;
    } else {
      return false;
    }
  }
}
