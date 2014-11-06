#include <hop_bits/optimisationProblem.hpp>

// C++ Standard Library
#include <string>
#include <limits>
#include <stdexcept>

namespace hop {
  OptimisationProblem::OptimisationProblem(const unsigned int& numberOfDimensions)
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

  // Checks for each dimension whether the parameter is greater or equal the lower bound.
  arma::Col<arma::uword> OptimisationProblem::isSatisfyingLowerBounds(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingLowerBounds_.find(parameter);
    if (cachePosition == cachedIsSatisfyingLowerBounds_.end()) {
      // If false: Compute which parameter violates the lower bound ...
      arma::Col<arma::uword> result = (getScaledCongruentParameter(parameter) >= lowerBounds_);

      // ... and add it to the cache afterwards.
      cachedIsSatisfyingLowerBounds_.insert({parameter, result});

      return result;
    } else {
      // If true: Return the cached result.
      return cachePosition->second;
    }
  }

  arma::Col<arma::uword> OptimisationProblem::isSatisfyingUpperBounds(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    auto cachePosition = cachedIsSatisfyingUpperBounds_.find(parameter);
    if (cachePosition == cachedIsSatisfyingUpperBounds_.end()) {
      // If false: Compute the result ...
      arma::Col<arma::uword> result = (getScaledCongruentParameter(parameter) <= upperBounds_);

      // ... and add it to the cache afterwards.
      cachedIsSatisfyingUpperBounds_.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  // Checks whether any soft-constraint is violated
  bool OptimisationProblem::isSatisfyingSoftConstraints(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    auto cachePosition = cachedIsSatisfyingSoftConstraints_.find(parameter);
    if (cachePosition == cachedIsSatisfyingSoftConstraints_.end()) {
      bool result = (getSoftConstraintsValue(parameter) == 0);

      cachedIsSatisfyingSoftConstraints_.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  // Checks whether any intervall- or soft-constraint is violated.
  bool OptimisationProblem::isSatisfyingConstraints(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    auto cachePosition = cachedIsSatisfyingConstraints_.find(parameter);
    if (cachePosition == cachedIsSatisfyingConstraints_.end()) {
      bool result = (all(isSatisfyingLowerBounds(parameter)) && all(isSatisfyingUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));

      cachedIsSatisfyingConstraints_.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  double OptimisationProblem::getSoftConstraintsValue(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    auto cachePosition = cachedSoftConstraintsValues_.find(parameter);
    if (cachePosition == cachedSoftConstraintsValues_.end()) {
      double result = getSoftConstraintsValueImplementation(getScaledCongruentParameter(parameter));

      cachedSoftConstraintsValues_.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  double OptimisationProblem::getObjectiveValue(const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    ++numberOfEvaluations_;
    auto cachePosition = cachedObjectiveValues_.find(parameter);
    if (cachePosition == cachedObjectiveValues_.end()) {
      ++numberOfDistinctEvaluations_;

      double result = getObjectiveValueImplementation(getScaledCongruentParameter(parameter)) + objectiveValueTranslation_;

      cachedObjectiveValues_.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  unsigned int OptimisationProblem::getNumberOfDimensions() const {
    return numberOfDimensions_;
  }

  arma::Col<double> OptimisationProblem::getLowerBounds() const {
    return lowerBounds_;
  }

  void OptimisationProblem::setLowerBounds(const arma::Col<double>& lowerBounds) {
    if (lowerBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the lower bound (" + std::to_string(lowerBounds.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    lowerBounds_ = lowerBounds;
  }

  arma::Col<double> OptimisationProblem::getUpperBounds() const {
    return upperBounds_;
  }

  void OptimisationProblem::setUpperBounds(const arma::Col<double>& upperBounds) {
    if (upperBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the upper bound (" + std::to_string(upperBounds.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    upperBounds_ = upperBounds;
  }

  void OptimisationProblem::setParameterTranslation(const arma::Col<double>& parameterTranslation) {
    if (parameterTranslation.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter translation (" + std::to_string(parameterTranslation.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterTranslation_ = parameterTranslation;
  }

  void OptimisationProblem::setParameterRotation(const arma::Mat<double>& parameterRotation) {
    parameterRotation_ = parameterRotation;
  }

  void OptimisationProblem::setParameterScale(const arma::Col<double>& parameterScale) {
    if (parameterScale.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter scale (" + std::to_string(parameterScale.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterScale_ = parameterScale;
  }

  void OptimisationProblem::setObjectiveValueTranslation(const double& objectiveValueTranslation) {
    objectiveValueTranslation_ = objectiveValueTranslation;
  }

  void OptimisationProblem::OptimisationProblem::setObjectiveValueScale(const double& objectiveValueScale) {
    objectiveValueScale_ = objectiveValueScale;
  }

  double OptimisationProblem::getAcceptableObjectiveValue() const {
    return acceptableObjectiveValue_;
  }

  void OptimisationProblem::setAcceptableObjectiveValue(const double& acceptableObjectiveValue) {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  unsigned int OptimisationProblem::getNumberOfEvaluations() const {
    return numberOfEvaluations_;
  }

  unsigned int OptimisationProblem::getNumberOfDistinctEvaluations() const {
    return numberOfDistinctEvaluations_;
  }

  void OptimisationProblem::reset() {
    numberOfEvaluations_ = 0;
    numberOfDistinctEvaluations_ = 0;

    // Add to comment in header "... and clearing the cache"
    cachedObjectiveValues_.clear();
    cachedSoftConstraintsValues_.clear();
    cachedIsSatisfyingLowerBounds_.clear();
    cachedIsSatisfyingUpperBounds_.clear();
    cachedIsSatisfyingSoftConstraints_.clear();
    cachedIsSatisfyingConstraints_.clear();
  }

  arma::Col<double> OptimisationProblem::getScaledCongruentParameter(const arma::Col<double>& parameter) const {
    return parameterRotation_ * parameterScale_ % (parameter + parameterTranslation_);
  }

  double OptimisationProblem::getSoftConstraintsValueImplementation(const arma::Col<double>& parameter) const {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    return 0;
  }

  std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> OptimisationProblem::getCachedObjectiveValues() const {
    return cachedObjectiveValues_;
  }

  std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> OptimisationProblem::getCachedSoftConstraintsValues() const {
    return cachedSoftConstraintsValues_;
  }

  std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, CacheHasher, CacheKeyEqual> OptimisationProblem::getCachedIsSatisfyingLowerBounds() const {
    return cachedIsSatisfyingLowerBounds_;
  }

  std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, CacheHasher, CacheKeyEqual> OptimisationProblem::getCachedIsSatisfyingUpperBounds() const {
    return cachedIsSatisfyingUpperBounds_;
  }

  std::unordered_map<arma::Col<double>, bool, CacheHasher, CacheKeyEqual> OptimisationProblem::getCachedIsSatisfyingSoftConstraints() const {
    return cachedIsSatisfyingSoftConstraints_;
  }

  std::unordered_map<arma::Col<double>, bool, CacheHasher, CacheKeyEqual> OptimisationProblem::getCachedIsSatisfyingConstraints() const {
    return cachedIsSatisfyingConstraints_;
  }
}
