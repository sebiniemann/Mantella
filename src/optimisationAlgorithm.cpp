#include "mantella_bits/optimisationAlgorithm.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <atomic>
#include <cassert>
#include <limits>

// Mantella
#include "mantella_bits/helper/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#if defined(SUPPORT_MPI)
#include "mantella_bits/helper/mpi.hpp" // IWYU pragma: keep
#endif

namespace mant {
  OptimisationAlgorithm::OptimisationAlgorithm()
      : numberOfIterations_(0),
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

  void OptimisationAlgorithm::optimise(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {
    verify(arma::all(optimisationProblem_->getLowerBounds() <= optimisationProblem_->getUpperBounds()), "All upper bounds of the optimisation problem must be greater than or equal to its lower bound.");

    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestParameter_.fill(std::numeric_limits<double>::quiet_NaN());
    numberOfIterations_ = 0;

    while (!isFinished() && !isTerminated) {
      arma::Mat<double> parameters = samplingFunction();

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        parameters.col(n) = boundaryHandlingFunction(parameters.col(n));
      }

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const double objectiveValue = optimisationProblem_->getObjectiveValue(parameter);
        if (recordSamples) {
          samplingHistory_.push_back({parameter, objectiveValue});
        };
      }

      if (objectiveValue < bestObjectiveValue_) {
        bestParameter_ = parameter;
        bestObjectiveValue_ = objectiveValue;

        return true;
      } else {
        return false;
      }
    }

#if defined(SUPPORT_MPI)
    MPI_Datatype MANT_MPI_PARAMETER;
    MPI_Type_contiguous(static_cast<int>(2 + numberOfDimensions_), MPI_DOUBLE, &MANT_MPI_PARAMETER);
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
}
