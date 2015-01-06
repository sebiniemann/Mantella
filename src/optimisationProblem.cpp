#include <hop_bits/optimisationProblem.hpp>

// C++ Standard Library
#include <string>
#include <limits>
#include <stdexcept>

namespace hop {
  template <>
  OptimisationProblem<double>::OptimisationProblem(
      const unsigned int& numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - std::numeric_limits<double>::max());
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + std::numeric_limits<double>::max());
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_));
    setParameterScale(arma::ones<arma::Col<double>>(numberOfDimensions_));
    setObjectiveValueTranslation(0.0);
    setObjectiveValueScale(1.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  template <>
  void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    if (parameterTranslation.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter translation (" + std::to_string(parameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterTranslation_ = parameterTranslation;
  }

  template <>
  void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double>& parameterRotation) {
    if (!parameterRotation.is_square()) {
      throw std::logic_error("The rotation matrix (" + std::to_string(parameterRotation.n_rows) + ", " + std::to_string(parameterRotation.n_cols) + ") must be square.");
    } else if (parameterRotation.n_rows != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(parameterRotation.n_rows) + ", " + std::to_string(parameterRotation.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    } else if(arma::any(arma::vectorise(arma::abs(parameterRotation.i() - parameterRotation.t()) > 1.0e-12)) || std::abs(std::abs(arma::det(parameterRotation)) - 1.0) > 1.0e-12) {
      throw std::logic_error("The rotation matrix must be orthonormal and its determinant (" + std::to_string(arma::det(parameterRotation)) + ") must be either 1 or -1.");
    }

    parameterRotation_ = parameterRotation;
  }

  template <>
  void OptimisationProblem<double>::setParameterScale(
      const arma::Col<double>& parameterScale) {
    if (parameterScale.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter scale (" + std::to_string(parameterScale.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterScale_ = parameterScale;
  }

  template <>
  double OptimisationProblem<double>::getObjectiveValue(
      const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Always increase the number of evaluations (whether its computed or retrived from cache).
    ++numberOfEvaluations_;

    // Check if the result is already cached.
    const auto& cachePosition = cachedObjectiveValues_.find(parameter);
    if (cachePosition == cachedObjectiveValues_.end()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      // The result was not found, compute it.
      const double& result = objectiveValueScale_ * getObjectiveValueImplementation(getScaledCongruentParameter(parameter)) + objectiveValueTranslation_;
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <>
  arma::Col<double> OptimisationProblem<double>::getScaledCongruentParameter(
      const arma::Col<double>& parameter) const noexcept {
    return parameterRotation_ * parameterScale_ % (parameter + parameterTranslation_);
  }
}
