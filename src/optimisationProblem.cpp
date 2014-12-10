#include <hop_bits/optimisationProblem.hpp>

// C++ Standard Library
#include <string>
#include <limits>
#include <stdexcept>

namespace hop {
  OptimisationProblem::OptimisationProblem(
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

  arma::Col<arma::uword> OptimisationProblem::isSatisfyingLowerBounds(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingLowerBounds_.find(parameter);
    if (cachePosition == cachedIsSatisfyingLowerBounds_.end()) {
      // The result was not found, compute it.
      const arma::Col<arma::uword>& result = (getScaledCongruentParameter(parameter) >= lowerBounds_);
      cachedIsSatisfyingLowerBounds_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  arma::Col<arma::uword> OptimisationProblem::isSatisfyingUpperBounds(
      const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingUpperBounds_.find(parameter);
    if (cachePosition == cachedIsSatisfyingUpperBounds_.end()) {
      // The result was not found, compute it.
      const arma::Col<arma::uword>& result = (getScaledCongruentParameter(parameter) <= upperBounds_);
      cachedIsSatisfyingUpperBounds_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  bool OptimisationProblem::isSatisfyingSoftConstraints(
      const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingSoftConstraints_.find(parameter);
    if (cachePosition == cachedIsSatisfyingSoftConstraints_.end()) {
      // The result was not found, compute it.
      const bool& result = (getSoftConstraintsValue(parameter) == 0);
      cachedIsSatisfyingSoftConstraints_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  bool OptimisationProblem::isSatisfyingConstraints(
      const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingConstraints_.find(parameter);
    if (cachePosition == cachedIsSatisfyingConstraints_.end()) {
      // The result was not found, compute it.
      const bool& result = (all(isSatisfyingLowerBounds(parameter)) && all(isSatisfyingUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));
      cachedIsSatisfyingConstraints_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  double OptimisationProblem::getSoftConstraintsValue(
      const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedSoftConstraintsValues_.find(parameter);
    if (cachePosition == cachedSoftConstraintsValues_.end()) {
      // The result was not found, compute it.
      const double& result = getSoftConstraintsValueImplementation(parameter);

      if(result < 0) {
        throw std::runtime_error("The soft constraint value (" + std::to_string(result) + ") must be greater or equal 0.");
      }

      cachedSoftConstraintsValues_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  double OptimisationProblem::getObjectiveValue(
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

  unsigned int OptimisationProblem::getNumberOfDimensions() const noexcept {
    return numberOfDimensions_;
  }

  arma::Col<double> OptimisationProblem::getLowerBounds() const noexcept {
    return lowerBounds_;
  }

  void OptimisationProblem::setLowerBounds(
      const arma::Col<double>& lowerBounds) {
    if (lowerBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the lower bound (" + std::to_string(lowerBounds.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    lowerBounds_ = lowerBounds;
  }

  arma::Col<double> OptimisationProblem::getUpperBounds() const noexcept {
    return upperBounds_;
  }

  void OptimisationProblem::setUpperBounds(const arma::Col<double>& upperBounds) {
    if (upperBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the upper bound (" + std::to_string(upperBounds.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    upperBounds_ = upperBounds;
  }

  void OptimisationProblem::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    if (parameterTranslation.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter translation (" + std::to_string(parameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterTranslation_ = parameterTranslation;
  }


  void OptimisationProblem::setParameterRotation(
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

  void OptimisationProblem::setParameterScale(
      const arma::Col<double>& parameterScale) {
    if (parameterScale.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter scale (" + std::to_string(parameterScale.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterScale_ = parameterScale;
  }

  void OptimisationProblem::setObjectiveValueTranslation(
      const double& objectiveValueTranslation) noexcept {
    objectiveValueTranslation_ = objectiveValueTranslation;
  }

  void OptimisationProblem::OptimisationProblem::setObjectiveValueScale(
      const double& objectiveValueScale) noexcept {
    objectiveValueScale_ = objectiveValueScale;
  }

  double OptimisationProblem::getAcceptableObjectiveValue() const noexcept {
    return acceptableObjectiveValue_;
  }

  void OptimisationProblem::setAcceptableObjectiveValue(
      const double& acceptableObjectiveValue) noexcept {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  unsigned int OptimisationProblem::getNumberOfEvaluations() const noexcept {
    return numberOfEvaluations_;
  }

  unsigned int OptimisationProblem::getNumberOfDistinctEvaluations() const noexcept {
    return numberOfDistinctEvaluations_;
  }

  void OptimisationProblem::reset() noexcept {
    numberOfEvaluations_ = 0;
    numberOfDistinctEvaluations_ = 0;

    cachedObjectiveValues_.clear();
    cachedSoftConstraintsValues_.clear();
    cachedIsSatisfyingLowerBounds_.clear();
    cachedIsSatisfyingUpperBounds_.clear();
    cachedIsSatisfyingSoftConstraints_.clear();
    cachedIsSatisfyingConstraints_.clear();
  }

  std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> OptimisationProblem::getCachedObjectiveValues() const {
    return cachedObjectiveValues_;
  }

  std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> OptimisationProblem::getCachedSoftConstraintsValues() const {
    return cachedSoftConstraintsValues_;
  }

  std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> OptimisationProblem::getCachedIsSatisfyingLowerBounds() const {
    return cachedIsSatisfyingLowerBounds_;
  }

  std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> OptimisationProblem::getCachedIsSatisfyingUpperBounds() const {
    return cachedIsSatisfyingUpperBounds_;
  }

  std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> OptimisationProblem::getCachedIsSatisfyingSoftConstraints() const {
    return cachedIsSatisfyingSoftConstraints_;
  }

  std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> OptimisationProblem::getCachedIsSatisfyingConstraints() const {
    return cachedIsSatisfyingConstraints_;
  }

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  double OptimisationProblem::getSoftConstraintsValueImplementation(
      const arma::Col<double>& parameter) const noexcept {
    return 0.0;
  }

  arma::Col<double> OptimisationProblem::getScaledCongruentParameter(
      const arma::Col<double>& parameter) const noexcept {
    return parameterRotation_ * parameterScale_ % (parameter + parameterTranslation_);
  }
}
