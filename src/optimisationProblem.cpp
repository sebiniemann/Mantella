#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <cassert>
#include <cmath>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  OptimisationProblem::OptimisationProblem(
      const arma::uword numberOfDimensions)
      : numberOfDimensions_(numberOfDimensions) {
    // We avoid infinite values for any information on the problem, that could be used within an optimisation algorithm.
    // Otherwise, we would need separate implementations to handle infinite boundaries.
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - 10);
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 10);

    setParameterPermutation(range(0, numberOfDimensions_ - 1));
    setParameterScaling(arma::ones<arma::Col<double>>(numberOfDimensions_));
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_));

    setObjectiveValueScaling(1.0);
    setObjectiveValueTranslation(0.0);

    setMinimalParameterDistance(arma::zeros<arma::Col<double>>(numberOfDimensions_));

    reset();
  }

  void OptimisationProblem::setObjectiveFunction(
      const std::function<double(const arma::Col<double>& parameter_)> objectiveFunction,
      const std::string& objectiveFunctionName) {
    verify(static_cast<bool>(objectiveFunction), "OptimisationProblem.setObjectiveFunction: The objective function must be callable.");

    objectiveFunction_ = objectiveFunction;
    objectiveFunctionName_ = objectiveFunctionName;

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setObjectiveFunction(
      const std::function<double(const arma::Col<double>& parameter_)> objectiveFunction) {
    setObjectiveFunction(objectiveFunction, "Unnamed, custom objective function");
  }

  std::string OptimisationProblem::getObjectiveFunctionName() const {
    return objectiveFunctionName_;
  }

  double OptimisationProblem::getObjectiveValue(
      const arma::Col<double>& parameter) {
    verify(static_cast<bool>(objectiveFunction_), "OptimisationProblem.getObjectiveValue: The objective function must be callable.");
    verify(parameter.n_elem == numberOfDimensions_, "OptimisationProblem.getObjectiveValue: The number of elements must be equal to the number of dimensions.");
    verify(parameter.is_finite(), "OptimisationProblem.getObjectiveValue: The parameter must be finite.");

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

    const arma::Col<double>& discretisedParameter = getDiscretisedParameter(parameter);
    if (::mant::isCachingSamples) {
      // Check if the result is already cached.
      const auto n = cachedSamples_.find(discretisedParameter);
      if (n == cachedSamples_.cend()) {
        // Increases the number of distinct evaluations only, if we actually compute the value.
        ++numberOfDistinctEvaluations_;

        const double result = getModifiedObjectiveValue(objectiveFunction_(getModifiedParameter(discretisedParameter)));
        // All objective values must be finite.
        assert(std::isfinite(result));

        cachedSamples_.insert({discretisedParameter, result});
        return result;
      } else {
        return n->second;
      }
    } else {
      // Without caching, all function evaluations must be computed.
      ++numberOfDistinctEvaluations_;

      const double result = getModifiedObjectiveValue(objectiveFunction_(getModifiedParameter(discretisedParameter)));
      // All objective values must be finite.
      assert(std::isfinite(result));

      return result;
    }
  }

  double OptimisationProblem::getNormalisedObjectiveValue(
      const arma::Col<double>& parameter) {
    return getObjectiveValue(lowerBounds_ + parameter % (upperBounds_ - lowerBounds_));
  }

  void OptimisationProblem::setLowerBounds(
      const arma::Col<double>& lowerBounds) {
    verify(lowerBounds.n_elem == numberOfDimensions_, "OptimisationProblem.setLowerBounds: The lower bounds' number of elements must be equal to the optimisation problem's number of dimensions.");
    verify(lowerBounds.is_finite(), "OptimisationProblem.setLowerBounds: The lower bounds must be finite.");

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
    verify(upperBounds.n_elem == numberOfDimensions_, "OptimisationProblem.setUpperBounds: The upper bounds' number of elements must be equal to the optimisation problem's number of dimensions.");
    verify(upperBounds.is_finite(), "OptimisationProblem.setUpperBounds: The upper bounds must be finite.");

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
    verify(parameterPermutation.n_elem == numberOfDimensions_, "OptimisationProblem.setParameterPermutation: The parameter permutation's number of elements must be equal to the optimisation problem's number of dimensions.");
    verify(isPermutationVector(parameterPermutation, numberOfDimensions_, numberOfDimensions_), "OptimisationProblem.setParameterPermutation: The (provided) parameter permutation must be an actual permutation on the optimisation problem.");

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

  arma::Col<arma::uword> OptimisationProblem::getParameterPermutation() const {
    return parameterPermutation_;
  }

  void OptimisationProblem::setParameterScaling(
      const arma::Col<double>& parameterScaling) {
    verify(parameterScaling.n_elem == numberOfDimensions_, "OptimisationProblem.setParameterScaling: The parameter scaling's number of elements must be equal to the optimisation problem's number of dimensions.");
    verify(parameterScaling.is_finite(), "OptimisationProblem.setParameterScaling: The parameter scaling must be finite.");

    parameterScaling_ = parameterScaling;
#if defined(SUPPORT_MPI)
    MPI_Bcast(parameterScaling_.memptr(), static_cast<int>(parameterScaling_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::Col<double> OptimisationProblem::getParameterScaling() const {
    return parameterScaling_;
  }

  void OptimisationProblem::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    verify(parameterTranslation.n_elem == numberOfDimensions_, "OptimisationProblem.setParameterTranslation: The parameter translation's number of elements must be equal to the optimisation problem's number of dimensions.");
    verify(parameterTranslation.is_finite(), "OptimisationProblem.setParameterTranslation: The parameter translation must be finite.");

    parameterTranslation_ = parameterTranslation;
#if defined(SUPPORT_MPI)
    MPI_Bcast(parameterTranslation_.memptr(), static_cast<int>(parameterTranslation_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::Col<double> OptimisationProblem::getParameterTranslation() const {
    return parameterTranslation_;
  }

  void OptimisationProblem::setParameterRotation(
      const arma::Mat<double>& parameterRotation) {
    verify(parameterRotation.n_rows == numberOfDimensions_, "OptimisationProblem.setParameterRotation: The parameter rotation's number of rows must be equal to the optimisation problem's number of dimensions.");
    verify(isRotationMatrix(parameterRotation), "OptimisationProblem.setParameterRotation: The (provided) parameter rotation must be an actual rotation matrix.");

    parameterRotation_ = parameterRotation;
#if defined(SUPPORT_MPI)
    MPI_Bcast(parameterRotation_.memptr(), static_cast<int>(parameterRotation_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::Mat<double> OptimisationProblem::getParameterRotation() const {
    return parameterRotation_;
  }

  void OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) {
    verify(std::isfinite(objectiveValueScaling), "OptimisationProblem.setObjectiveValueScaling: The objective value scaling must be finite.");

    objectiveValueScaling_ = objectiveValueScaling;
#if defined(SUPPORT_MPI)
    MPI_Bcast(&objectiveValueScaling_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  double OptimisationProblem::getObjectiveValueScaling() const {
    return objectiveValueScaling_;
  }

  void OptimisationProblem::setObjectiveValueTranslation(
      const double objectiveValueTranslation) {
    verify(std::isfinite(objectiveValueTranslation), "OptimisationProblem.setObjectiveValueTranslation: The objective value translation must be finite.");

    objectiveValueTranslation_ = objectiveValueTranslation;
#if defined(SUPPORT_MPI)
    MPI_Bcast(&objectiveValueTranslation_, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  double OptimisationProblem::getObjectiveValueTranslation() const {
    return objectiveValueTranslation_;
  }

  std::unordered_map<arma::Col<double>, double, Hash, IsEqual> OptimisationProblem::getCachedSamples() const {
    return cachedSamples_;
  }

  void OptimisationProblem::setMinimalParameterDistance(
      const arma::Col<double>& minimalParameterDistance) {
    verify(minimalParameterDistance.n_elem == numberOfDimensions_, "OptimisationProblem.setMinimalParameterDistance: The minimal parameter distance's number of elements must be equal to the optimisation problem's number of dimensions.");
    verify(arma::all(minimalParameterDistance >= 0), "OptimisationProblem.setMinimalParameterDistance: Each minimal parameter distance must be positive (including 0).");
    verify(minimalParameterDistance.is_finite(), "OptimisationProblem.setMinimalParameterDistance: The minimal parameter distance must be finite.");

    minimalParameterDistance_ = minimalParameterDistance;
#if defined(SUPPORT_MPI)
    MPI_Bcast(minimalParameterDistance_.memptr(), static_cast<int>(minimalParameterDistance_.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
#endif
  }

  arma::Col<double> OptimisationProblem::getMinimalParameterDistance() const {
    return minimalParameterDistance_;
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

  arma::Col<double> OptimisationProblem::getDiscretisedParameter(
      const arma::Col<double>& parameter) const {
    assert(parameter.n_elem == numberOfDimensions_);

    arma::Col<double> discretisedParameter = parameter;
    const arma::Col<arma::uword>& elementsToDiscretise = arma::find(minimalParameterDistance_ > 0);
    discretisedParameter.elem(elementsToDiscretise) = arma::floor(discretisedParameter.elem(elementsToDiscretise) / minimalParameterDistance_.elem(elementsToDiscretise)) % minimalParameterDistance_.elem(elementsToDiscretise);

    return discretisedParameter;
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
