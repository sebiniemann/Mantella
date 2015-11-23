#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>
#include <cmath>
#include <limits>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  OptimisationProblem::OptimisationProblem(
      const arma::uword numberOfDimensions)
      : numberOfDimensions_(numberOfDimensions) {
    reset();

    // We avoid infinite values for any information on the problem, that could be used with an optimisation algorithm.
    // Otherwise, we would need separate implementations to handle infinite boundaries.
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - std::numeric_limits<double>::max());
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + std::numeric_limits<double>::max());

    setParameterPermutation(range<arma::uword>(0, numberOfDimensions_ - 1));
    setParameterScaling(arma::ones<arma::Col<double>>(numberOfDimensions_));
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_));

    setObjectiveValueScaling(1.0);
    setObjectiveValueTranslation(0.0);

    setAcceptableObjectiveValueThreshold(std::numeric_limits<double>::lowest());
  }

  void OptimisationProblem::setObjectiveFunction(
      const std::function<double(const arma::Col<double>& parameter)> objectiveFunction,
      const std::string& name) {
    objectiveFunction_ = objectiveFunction;
    name_ = name;

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setObjectiveFunction(
      const std::function<double(const arma::Col<double>& parameter)> objectiveFunction) {
    setObjectiveFunction(objectiveFunction, "Unnamed custom problem");
  }

  double OptimisationProblem::getObjectiveValue(
      const arma::Col<double>& parameter) {
    // Using the *operator bool* to checks whether *objectiveFunction_* is empty (not callable) or not.
    verify(static_cast<bool>(objectiveFunction_), "getObjectiveValue: The objective function is not callable.");
    verify(parameter.n_elem == numberOfDimensions_, "getObjectiveValue: The number of elements must be equal to the number of dimensions.");

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

    if (mant::cacheSamples) {
      // Check if the result is already cached.
      const auto n = cachedSamples_.find(parameter);
      if (n == cachedSamples_.cend()) {
        // Increase the number of distinct evaluations only if we actually compute the value.
        ++numberOfDistinctEvaluations_;

        const double result = getModifiedObjectiveValue(objectiveFunction_(getModifiedParameter(parameter)));
        // All objective values must be finite.
        assert(std::isfinite(result));

        cachedSamples_.insert({parameter, result});
        return result;
      } else {
        return n->second;
      }
    } else {
      // Without caching, all function evaluations must be computed.
      ++numberOfDistinctEvaluations_;

      return getModifiedObjectiveValue(objectiveFunction_(getModifiedParameter(parameter)));
    }
  }

  void OptimisationProblem::setLowerBounds(
      const arma::Col<double>& lowerBounds) {
    verify(lowerBounds.n_elem == numberOfDimensions_, "setLowerBounds: The number of elements within the lower bound must be equal to the number of problem dimensions.");
    verify(lowerBounds.is_finite(), "setLowerBounds: All elements within the lower bound must be finite.");

    lowerBounds_ = lowerBounds;
#if defined(SUPPORT_MPI)
    MPI_Bcast(lowerBounds_.memptr(), static_cast<int>(lowerBounds_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
  }

  arma::Col<double> OptimisationProblem::getLowerBounds() const {
    return lowerBounds_;
  }

  void OptimisationProblem::setUpperBounds(
      const arma::Col<double>& upperBounds) {
    verify(upperBounds.n_elem == numberOfDimensions_, "setUpperBounds: The number of elements within the upper bound must be equal to the number of problem dimensions.");
    verify(upperBounds.is_finite(), "setUpperBounds: All elements within the upper bound must be finite.");

    upperBounds_ = upperBounds;
#if defined(SUPPORT_MPI)
    MPI_Bcast(upperBounds_.memptr(), static_cast<int>(upperBounds_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
  }

  arma::Col<double> OptimisationProblem::getUpperBounds() const {
    return upperBounds_;
  }

  void OptimisationProblem::setParameterPermutation(
      const arma::Col<arma::uword>& parameterPermutation) {
    verify(parameterPermutation.n_elem == numberOfDimensions_, "setParameterPermutation: The number of elements must be equal to the number of dimensions");
    verify(isPermutationMatrix(parameterPermutation, numberOfDimensions_, numberOfDimensions_), "setParameterPermutation: The parameter must be a permutation.");

#if defined(SUPPORT_MPI)
    // MPI needs the exact type (here: *MPI_UNSIGNED*), while Armadillo's *arma::uword* is implementation dependent.
    // Therefore, we convert the permutation matrix to a specific type first, ...
    arma::Col<unsigned int> covertedParameterPermutation = arma::conv_to<arma::Col<unsigned int>>::from(parameterPermutation);
    // ... synchronise it over MPI ...
    MPI_Bcast(covertedParameterPermutation.memptr(), static_cast<int>(covertedParameterPermutation.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    // ... and reconvert/store is afterwards.
    parameterPermutation_ = arma::conv_to<arma::Col<arma::uword>>::from(covertedParameterPermutation);
#else
    parameterPermutation_ = parameterPermutation;
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setParameterScaling(
      const arma::Col<double>& parameterScaling) {
    verify(parameterScaling.n_elem == numberOfDimensions_, "setParameterScaling: The number of elements must be equal to the number of dimensions.");
    verify(parameterScaling.is_finite(), "setParameterScaling: All elements must be finite.");

    parameterScaling_ = parameterScaling;
#if defined(SUPPORT_MPI)
    MPI_Bcast(parameterScaling_.memptr(), static_cast<int>(parameterScaling_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    verify(parameterTranslation.n_elem == numberOfDimensions_, "setParameterTranslation: The number of elements must be equal to the number of dimensions.");
    verify(parameterTranslation.is_finite(), "setParameterTranslation: All elements must be finite.");

    parameterTranslation_ = parameterTranslation;
#if defined(SUPPORT_MPI)
    MPI_Bcast(parameterTranslation_.memptr(), static_cast<int>(parameterTranslation_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setParameterRotation(
      const arma::Mat<double>& parameterRotation) {
    verify(parameterRotation.n_rows == numberOfDimensions_, "setParameterRotation: The number of rows must be equal to the number of dimensions.");
    verify(isRotationMatrix(parameterRotation), "setParameterRotation: The parameter must be a rotation matrix.");

    parameterRotation_ = parameterRotation;
#if defined(SUPPORT_MPI)
    MPI_Bcast(parameterRotation_.memptr(), static_cast<int>(parameterRotation_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) {
    verify(std::isfinite(objectiveValueScaling), "setObjectiveValueScaling: The objective value scaling must be finite.");

    objectiveValueScaling_ = objectiveValueScaling;
#if defined(SUPPORT_MPI)
    MPI_Bcast(&objectiveValueScaling_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setObjectiveValueTranslation(
      const double objectiveValueTranslation) {
    verify(std::isfinite(objectiveValueTranslation), "setObjectiveValueTranslation: The objective value translation must be finite.");

    objectiveValueTranslation_ = objectiveValueTranslation;
#if defined(SUPPORT_MPI)
    MPI_Bcast(&objectiveValueTranslation_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setAcceptableObjectiveValueThreshold(
      const double acceptableObjectiveValueThreshold) {
    acceptableObjectiveValueThreshold_ = acceptableObjectiveValueThreshold;
#if defined(SUPPORT_MPI)
    MPI_Bcast(&acceptableObjectiveValueThreshold_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
  }

  double OptimisationProblem::getAcceptableObjectiveValueThreshold() const {
    return acceptableObjectiveValueThreshold_;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> OptimisationProblem::getCachedSamples() const {
    return cachedSamples_;
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

  arma::Col<double> OptimisationProblem::getModifiedParameter(
      const arma::Col<double>& parameter) const {
    assert(parameter.n_elem == numberOfDimensions_);

    return parameterRotation_ * (parameterScaling_ % parameter.elem(parameterPermutation_) - parameterTranslation_);
  }

  double OptimisationProblem::getModifiedObjectiveValue(
      const double objectiveValue) const {
    return objectiveValueScaling_ * objectiveValue + objectiveValueTranslation_;
  }
}
