#include "mantella_bits/optimisationAlgorithm/gridSearch.hpp"

// C++ standard library
#include <vector>
// IWYU pragma: no_include <ext/alloc_traits.h>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  GridSearch::GridSearch()
      : OptimisationAlgorithm() {
    setNextParametersFunction([this] (
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& parameters,
        const arma::Col<double>& objectiveValues,
        const arma::Col<double>& differences) {
      arma::Col<double> nextParameter(parameters.n_rows);
      for (arma::uword k = 0; k < sampleIndicies_.n_elem; ++k) {
        nextParameter(k) = samples_.at(k)(sampleIndicies_(k));
      }

      ++sampleIndicies_(0);
      for (arma::uword k = 0; k < sampleIndicies_.n_elem - 1; ++k) {
        if (sampleIndicies_(k) >= numberOfSamplesPerDimension_(k)) {
          sampleIndicies_(k) = 0;
          ++sampleIndicies_(k + 1);
        } else {
          break;
        }
      }
      
      return nextParameter;
    });
  }
  
  void GridSearch::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    if (numberOfSamplesPerDimension_.is_empty()) {
      setNumberOfSamplesPerDimension(arma::zeros<arma::Col<arma::uword>>(numberOfDimensions) + 10);
    } else {
      verify(arma::prod(numberOfSamplesPerDimension_) <= maximalNumberOfIterations_, "optimise: The maximal number of iterations must be at least equal to the product over the number of samples per dimensions.");
    }
    
    samples_.reserve(numberOfDimensions);
    for (arma::uword n = 0; n < numberOfDimensions; ++n) {
      samples_.push_back(arma::linspace<arma::Col<double>>(0, 1, numberOfSamplesPerDimension_(n)));
    }
    
    sampleIndicies_ = arma::zeros<arma::Col<arma::uword>>(numberOfDimensions);
  }
  
  void GridSearch::optimise(
      OptimisationProblem& optimisationProblem) {
    OptimisationAlgorithm::optimise(optimisationProblem, arma::Mat<double>(optimisationProblem.numberOfDimensions_, 0));
  }

  void GridSearch::setNumberOfSamplesPerDimension(
      const arma::Col<arma::uword>& numberOfSamplesPerDimension) {
    verify(arma::all(numberOfSamplesPerDimension_ > 1), "optimise: The number of samples per dimensions must be strict greater than 1 for each dimension.");
      
    numberOfSamplesPerDimension_ = numberOfSamplesPerDimension;
  }
}
