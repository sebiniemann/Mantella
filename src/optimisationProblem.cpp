#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>
#include <atomic>
#include <cmath>
#include <limits>
#include <utility>

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  OptimisationProblem::OptimisationProblem(
      const arma::uword numberOfDimensions)
      : numberOfDimensions_(numberOfDimensions) {
    reset();

    // A vector with all elements set to the lowest representable value.
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - std::numeric_limits<double>::max());
    // A vector with all elements set to the largest representable value.
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + std::numeric_limits<double>::max());

    // (0, ..., numberOfDimensions - 1)
    setParameterPermutation(arma::linspace<arma::Col<arma::uword>>(0, numberOfDimensions_ - 1, numberOfDimensions));

    setParameterScaling(arma::ones<arma::Col<double>>(numberOfDimensions_));
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_));

    setObjectiveValueScaling(1.0);
    setObjectiveValueTranslation(0.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  void OptimisationProblem::setLowerBounds(
      const arma::Col<double>& lowerBounds) {
    verify(lowerBounds.n_elem == numberOfDimensions_, "The number of elements within the lower bound must be equal to the number of problem dimensions.");
    verify(lowerBounds.is_finite(), "All elements within the lower bound must be finite.");

    lowerBounds_ = lowerBounds;
  }

  void OptimisationProblem::setUpperBounds(
      const arma::Col<double>& upperBounds) {
    verify(upperBounds.n_elem == numberOfDimensions_, "The number of elements within the upper bound must be equal to the number of problem dimensions.");
    verify(upperBounds.is_finite(), "All elements within the upper bound must be finite.");

    upperBounds_ = upperBounds;
  }

  arma::Col<double> OptimisationProblem::getLowerBounds() const {
    return lowerBounds_;
  }

  arma::Col<double> OptimisationProblem::getUpperBounds() const {
    return upperBounds_;
  }

  arma::Col<arma::uword> OptimisationProblem::isWithinLowerBounds(
      const arma::Col<double>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return parameter >= lowerBounds_;
  }

  arma::Col<arma::uword> OptimisationProblem::isWithinUpperBounds(
      const arma::Col<double>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return parameter <= upperBounds_;
  }

  void OptimisationProblem::setParameterPermutation(
      const arma::Col<arma::uword>& parameterPermutation) {
    verify(parameterPermutation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions");
    verify(isPermutation(parameterPermutation, numberOfDimensions_, numberOfDimensions_), "The parameter must be a permutation.");

    parameterPermutation_ = parameterPermutation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  void OptimisationProblem::setParameterScaling(
      const arma::Col<double>& parameterScaling) {
    verify(parameterScaling.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");
    verify(parameterScaling.is_finite(), "All elements must be finite.");

    parameterScaling_ = parameterScaling;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  void OptimisationProblem::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    verify(parameterTranslation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");
    verify(parameterTranslation.is_finite(), "All elements must be finite.");

    parameterTranslation_ = parameterTranslation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  void OptimisationProblem::setParameterRotation(
      const arma::Mat<double>& parameterRotation) {
    verify(parameterRotation.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions.");
    verify(isRotationMatrix(parameterRotation), "The parameter must be a rotation matrix.");

    parameterRotation_ = parameterRotation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  void OptimisationProblem::OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) {
    verify(std::isfinite(objectiveValueScaling), "The objective value scaling must be finite.");

    objectiveValueScaling_ = objectiveValueScaling;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  void OptimisationProblem::setObjectiveValueTranslation(
      const double objectiveValueTranslation) {
    verify(std::isfinite(objectiveValueTranslation), "The objective value translation must be finite.");

    objectiveValueTranslation_ = objectiveValueTranslation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  void OptimisationProblem::setAcceptableObjectiveValue(
      const double acceptableObjectiveValue) {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  double OptimisationProblem::getAcceptableObjectiveValue() const {
    return acceptableObjectiveValue_;
  }

  double OptimisationProblem::getObjectiveValue(
      const arma::Col<double>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

    if (mant::cacheSamples) {
      // Check if the result is already cached.
      const auto n = cachedSamples_.find(parameter);
      if (n == cachedSamples_.cend()) {
        // Increase the number of distinct evaluations only if we actually compute the value.
        ++numberOfDistinctEvaluations_;

        const double result = objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
        assert(std::isfinite(result));

        cachedSamples_.insert({parameter, result});
        return result;
      } else {
        return n->second;
      }
    } else {
      // Without caching, all function evaluations must be computed.
      ++numberOfDistinctEvaluations_;

      return objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
    }
  }

  arma::uword OptimisationProblem::getNumberOfEvaluations() const {
    return numberOfEvaluations_;
  }

  arma::uword OptimisationProblem::getNumberOfDistinctEvaluations() const {
    return numberOfDistinctEvaluations_;
  }

  void OptimisationProblem::reset() {
    numberOfEvaluations_ = 0;
    numberOfDistinctEvaluations_ = 0;

    cachedSamples_.clear();
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> OptimisationProblem::getCachedSamples() const {
    return cachedSamples_;
  }

  arma::Col<double> OptimisationProblem::getDiversifiedParameter(
      const arma::Col<double>& parameter) const {
    assert(parameter.n_elem == numberOfDimensions_);

    return parameterRotation_ * (parameterScaling_ % parameter.elem(parameterPermutation_) - parameterTranslation_);
  }

#if defined(SUPPORT_MPI)
  std::vector<double> OptimisationProblem::serialise() const {
    std::vector<double> serialisedOptimisationProblem;

    for (arma::uword n = 0; n < lowerBounds_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(lowerBounds_(n));
    }

    for (arma::uword n = 0; n < upperBounds_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(upperBounds_(n));
    }

    for (arma::uword n = 0; n < parameterPermutation_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(parameterPermutation_(n)));
    }

    for (arma::uword n = 0; n < parameterScaling_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(parameterScaling_(n));
    }

    for (arma::uword n = 0; n < parameterTranslation_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(parameterTranslation_(n));
    }

    for (arma::uword n = 0; n < parameterRotation_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(parameterRotation_(n));
    }

    serialisedOptimisationProblem.push_back(objectiveValueScaling_);
    serialisedOptimisationProblem.push_back(objectiveValueTranslation_);

    serialisedOptimisationProblem.push_back(acceptableObjectiveValue_);

    return serialisedOptimisationProblem;
  }

  void OptimisationProblem::deserialise(
      std::vector<double> serialisedOptimisationProblem) {
    acceptableObjectiveValue_ = serialisedOptimisationProblem.back();
    serialisedOptimisationProblem.pop_back();

    objectiveValueScaling_ = serialisedOptimisationProblem.back();
    serialisedOptimisationProblem.pop_back();
    objectiveValueTranslation_ = serialisedOptimisationProblem.back();
    serialisedOptimisationProblem.pop_back();

    parameterRotation_.set_size(numberOfDimensions_, numberOfDimensions_);
    for (arma::uword n = 0; n < parameterRotation_.n_elem; ++n) {
      parameterRotation_(n) = serialisedOptimisationProblem.back();
      serialisedOptimisationProblem.pop_back();
    }

    parameterTranslation_.set_size(numberOfDimensions_);
    for (arma::uword n = 0; n < parameterTranslation_.n_elem; ++n) {
      parameterTranslation_(n) = serialisedOptimisationProblem.back();
      serialisedOptimisationProblem.pop_back();
    }

    parameterScaling_.set_size(numberOfDimensions_);
    for (arma::uword n = 0; n < parameterScaling_.n_elem; ++n) {
      parameterScaling_(n) = serialisedOptimisationProblem.back();
      serialisedOptimisationProblem.pop_back();
    }

    parameterPermutation_.set_size(numberOfDimensions_);
    for (arma::uword n = 0; n < parameterPermutation_.n_elem; ++n) {
      parameterPermutation_(n) = static_cast<arma::uword>(serialisedOptimisationProblem.back());
      serialisedOptimisationProblem.pop_back();
    }

    upperBounds_.set_size(numberOfDimensions_);
    for (arma::uword n = 0; n < upperBounds_.n_elem; ++n) {
      upperBounds_(n) = serialisedOptimisationProblem.back();
      serialisedOptimisationProblem.pop_back();
    }

    lowerBounds_.set_size(numberOfDimensions_);
    for (arma::uword n = 0; n < lowerBounds_.n_elem; ++n) {
      lowerBounds_(n) = serialisedOptimisationProblem.back();
      serialisedOptimisationProblem.pop_back();
    }
  }
#endif
}
