#include <hop_bits/optimisationProblem.hpp>

#include <string>
#include <limits>
#include <stdexcept>

#include <hop_bits/evaluation/monitor.hpp>

namespace hop {
  OptimisationProblem::OptimisationProblem(const unsigned int& numberOfDimensions) :
    _numberOfDimensions(numberOfDimensions),
    _numberOfEvaluations(0)
  {
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) - std::numeric_limits<double>::max());
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions) + std::numeric_limits<double>::max());
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions, numberOfDimensions));
    setParameterScale(arma::ones<arma::Col<double>>(numberOfDimensions));
    setObjectiveValueTranslation(0.0);
    setObjectiveValueScale(1.0);
    setMaximalNumberOfEvaluations(1000);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  arma::Col<arma::uword> OptimisationProblem::isSatisfyingLowerBounds(const arma::Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedIsSatisfyingLowerBounds.find(parameter);
    if(cachePosition == _cachedIsSatisfyingLowerBounds.end()) {
      arma::Col<arma::uword> result = (getScaledCongruentParameter(parameter) >= _lowerBounds);

      _cachedIsSatisfyingLowerBounds.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  arma::Col<arma::uword> OptimisationProblem::isSatisfyingUpperBounds(const arma::Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedIsSatisfyingUpperBounds.find(parameter);
    if(cachePosition == _cachedIsSatisfyingUpperBounds.end()) {
      arma::Col<arma::uword> result = (getScaledCongruentParameter(parameter) <= _upperBounds);

      _cachedIsSatisfyingUpperBounds.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  bool OptimisationProblem::isSatisfyingSoftConstraints(const arma::Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedIsSatisfyingSoftConstraints.find(parameter);
    if(cachePosition == _cachedIsSatisfyingSoftConstraints.end()) {
      bool result = (getSoftConstraintsValue(parameter) == 0);

      _cachedIsSatisfyingSoftConstraints.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  bool OptimisationProblem::isSatisfyingConstraints(const arma::Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedIsSatisfyingConstraints.find(parameter);
    if(cachePosition == _cachedIsSatisfyingConstraints.end()) {
      bool result = (all(isSatisfyingLowerBounds(parameter)) && all(isSatisfyingUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));

      _cachedIsSatisfyingConstraints.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  double OptimisationProblem::getSoftConstraintsValue(const arma::Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedSoftConstraintsValues.find(parameter);
    if(cachePosition == _cachedSoftConstraintsValues.end()) {
      double result = getSoftConstraintsValueImplementation(getScaledCongruentParameter(parameter));

      _cachedSoftConstraintsValues.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  double OptimisationProblem::getObjectiveValue(const arma::Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    _numberOfEvaluations++;
    Monitor::addNumberOfEvaluations(_numberOfEvaluations);

    auto cachePosition = _cachedObjectivValues.find(parameter);
    if(cachePosition == _cachedObjectivValues.end()) {
      double result = getObjectiveValueImplementation(getScaledCongruentParameter(parameter)) + _objectiveValueTranslation;

      _cachedObjectivValues.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  unsigned int OptimisationProblem::getNumberOfDimensions() const {
    return _numberOfDimensions;
  }

  arma::Col<double> OptimisationProblem::getLowerBounds() const {
    return _lowerBounds;
  }

  void OptimisationProblem::setLowerBounds(const arma::Col<double>& lowerBounds) {
    if (lowerBounds.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the lower bound (" + std::to_string(lowerBounds.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    _lowerBounds = lowerBounds;
  }

  arma::Col<double> OptimisationProblem::getUpperBounds() const {
    return _upperBounds;
  }

  void OptimisationProblem::setUpperBounds(const arma::Col<double>& upperBounds) {
    if (upperBounds.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the upper bound (" + std::to_string(upperBounds.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    _upperBounds = upperBounds;
  }

  void OptimisationProblem::setParameterTranslation(const arma::Col<double>& parameterTranslation) {
    if (parameterTranslation.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter translation (" + std::to_string(parameterTranslation.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    _parameterTranslation = parameterTranslation;
  }

  void OptimisationProblem::setParameterRotation(const arma::Mat<double>& parameterRotation) {
    _parameterRotation = parameterRotation;
  }

  void OptimisationProblem::setParameterScale(const arma::Col<double>& parameterScale) {
    if (parameterScale.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter scale (" + std::to_string(parameterScale.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    _parameterScale = parameterScale;
  }

  void OptimisationProblem::setObjectiveValueTranslation(const double& objectiveValueTranslation) {
    _objectiveValueTranslation = objectiveValueTranslation;
  }

  void OptimisationProblem::OptimisationProblem::setObjectiveValueScale(const double& objectiveValueScale) {
    _objectiveValueScale = objectiveValueScale;
  }

  unsigned int OptimisationProblem::getMaximalNumberOfEvaluations() const {
    return _maximalNumberOfEvaluations;
  }

  void OptimisationProblem::setMaximalNumberOfEvaluations(const unsigned int& maximalNumberOfEvaluations) {
    _maximalNumberOfEvaluations = maximalNumberOfEvaluations;
  }

  double OptimisationProblem::getAcceptableObjectiveValue() const {
    return _acceptableObjectiveValue;
  }

  void OptimisationProblem::setAcceptableObjectiveValue(const double& acceptableObjectiveValue) {
    _acceptableObjectiveValue = acceptableObjectiveValue;
  }

  unsigned int OptimisationProblem::getNumberOfEvaluations() const {
    return _numberOfEvaluations;
  }

  arma::Col<double> OptimisationProblem::getScaledCongruentParameter(const arma::Col<double>& parameter) const {
    return _parameterRotation * _parameterScale % (parameter + _parameterTranslation);
  }

  double OptimisationProblem::getSoftConstraintsValueImplementation(const arma::Col<double>& parameter) const {
    if(parameter.n_elem != _numberOfDimensions) {
      throw std::logic_error("The dimension of the parameter (" + std::to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + std::to_string(_numberOfDimensions) + ").");
    }

    return 0;
  }
}
