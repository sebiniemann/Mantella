#include <mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm/gridSearch.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  GridSearch::GridSearch(
      const std::shared_ptr<OptimisationProblem> optimisationProblem)
      : SamplingBasedOptimisationAlgorithm(optimisationProblem) {
    setNumberOfSamplesPerDimension(arma::zeros<arma::Col<arma::uword>>(numberOfDimensions_) + 10);
  }

  void GridSearch::optimiseImplementation() {
    verify(arma::prod(numberOfSamplesPerDimension_) <= maximalNumberOfIterations_ * static_cast<arma::uword>(numberOfNodes_),
        "The product of all number of samples per dimension must be less than the maximal number of iterations.");

    std::vector<arma::Col<double>> samples;
    for (arma::uword n = 0; n < numberOfDimensions_; ++n) {
      samples.push_back(arma::linspace<arma::Col<double>>(getLowerBounds()(n), getUpperBounds()(n), numberOfSamplesPerDimension_(n)));
    }

    arma::Col<arma::uword> sampleIndicies = arma::zeros<arma::Col<arma::uword>>(numberOfDimensions_);
    for (arma::uword n = 0; n < arma::prod(numberOfSamplesPerDimension_); ++n) {
      if (static_cast<int>(n) % numberOfNodes_ == nodeRank_) {
        ++numberOfIterations_;

        arma::Col<double> candidateParameter(numberOfDimensions_);
        for (arma::uword k = 0; k < sampleIndicies.n_elem; ++k) {
          candidateParameter(k) = samples.at(k)(sampleIndicies(k));
        }

        updateBestParameter(candidateParameter, getObjectiveValue(candidateParameter));

        if (isFinished()) {
          break;
        }
      }

      ++sampleIndicies(0);
      for (arma::uword k = 0; k < sampleIndicies.n_elem - 1; ++k) {
        if (sampleIndicies(k) >= numberOfSamplesPerDimension_(k)) {
          sampleIndicies(k) = 0;
          ++sampleIndicies(k + 1);
        } else {
          break;
        }
      }
    }
  }

  void GridSearch::setNumberOfSamplesPerDimension(
      const arma::Col<arma::uword>& numberOfSamplesPerDimension) {
    verify(numberOfSamplesPerDimension.n_elem == numberOfDimensions_,
        "The number of elements must be equal to the number of dimensions.");
    verify(arma::all(numberOfSamplesPerDimension > 1),
        "");  // TODO

    numberOfSamplesPerDimension_ = numberOfSamplesPerDimension;
  }

  arma::Col<arma::uword> GridSearch::getNumberOfSamplesPerDimension() const {
    return numberOfSamplesPerDimension_;
  }

  std::string GridSearch::toString() const {
    return "grid_search";
  }
}
