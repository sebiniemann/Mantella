#include <optimisationProblem/optimisationProblem.hpp>

#include <string>
using std::to_string;

#include <stdexcept>
using std::runtime_error;
using std::logic_error;

#include <iostream>
using std::cout;
using std::endl;

namespace hop {
  OptimisationProblem::OptimisationProblem(const unsigned int& numberOfDimensions) :
    _numberOfDimensions(numberOfDimensions),
    _lowerBounds(Col<double>(numberOfDimensions)),
    _upperBounds(Col<double>(numberOfDimensions)),
    _parameterTranslation(Col<double>(numberOfDimensions)),
    _parameterRotation(Mat<double>(numberOfDimensions, numberOfDimensions)),
    _parameterReflection(Col<double>(numberOfDimensions)),
    _parameterScale(Mat<double>(numberOfDimensions, numberOfDimensions)),
    _objectiveValueTranslation(0.0),
    _objectiveValueScale(1.0),
    _maximalNumberOfEvaluations(1000),
    _acceptableObjectiveValue(numeric_limits<double>::lowest()),
    _numberOfEvaluations(0)
  {
    _lowerBounds.fill(numeric_limits<double>::lowest());
    _upperBounds.fill(numeric_limits<double>::max());

    _parameterTranslation.zeros();
    _parameterRotation.eye();
    _parameterReflection.ones();
    _parameterScale.eye();
  }

  Col<uword> OptimisationProblem::isSatisfyingLowerBounds(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedIsSatisfyingLowerBounds.find(parameter);
    if(cachePosition == _cachedIsSatisfyingLowerBounds.end()) {
      Col<uword> result = (getScaledCongruentParameter(parameter) >= _lowerBounds);

      _cachedIsSatisfyingLowerBounds.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  Col<uword> OptimisationProblem::isSatisfyingUpperBounds(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    auto cachePosition = _cachedIsSatisfyingUpperBounds.find(parameter);
    if(cachePosition == _cachedIsSatisfyingUpperBounds.end()) {
      Col<uword> result = (getScaledCongruentParameter(parameter) <= _upperBounds);

      _cachedIsSatisfyingUpperBounds.insert({parameter, result});

      return result;
    } else {
      return cachePosition->second;
    }
  }

  bool OptimisationProblem::isSatisfyingSoftConstraints(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
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

  bool OptimisationProblem::isSatisfyingConstraints(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
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

  double OptimisationProblem::getSoftConstraintsValue(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
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

  double OptimisationProblem::getObjectiveValue(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    if(_numberOfEvaluations >= _maximalNumberOfEvaluations) {
      throw runtime_error("Reached maximal number of evaluations (" + to_string(_maximalNumberOfEvaluations) + ").");
    }

    _numberOfEvaluations++;

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

  Col<double> OptimisationProblem::getLowerBounds() const {
    return _lowerBounds;
  }

  void OptimisationProblem::setLowerBounds(const Col<double>& lowerBounds) {
    if (lowerBounds.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the lower bound (" + to_string(lowerBounds.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    _lowerBounds = lowerBounds;
  }

  Col<double> OptimisationProblem::getUpperBounds() const {
    return _upperBounds;
  }

  void OptimisationProblem::setUpperBounds(const Col<double>& upperBounds) {
    if (upperBounds.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the upper bound (" + to_string(upperBounds.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    _upperBounds = upperBounds;
  }

  void OptimisationProblem::setParameterTranslation(const Col<double>& parameterTranslation) {
    if (parameterTranslation.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter translation (" + to_string(parameterTranslation.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    _parameterTranslation = parameterTranslation;
  }

  void OptimisationProblem::setParameterRotation(const Mat<double>& parameterRotation) {
    _parameterRotation = parameterRotation;
  }

  void OptimisationProblem::setParameterReflection(const Col<double>& parameterReflection) {
    _parameterReflection = parameterReflection;
  }

  void OptimisationProblem::setParameterScale(const Mat<double>& parameterScale) {
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

  Col<double> OptimisationProblem::getScaledCongruentParameter(const Col<double>& parameter) const {
    return _parameterScale * _parameterRotation * (parameter % _parameterReflection + _parameterTranslation);
  }

  double OptimisationProblem::getSoftConstraintsValueImplementation(const Col<double>& parameter) {
    if(parameter.n_elem != _numberOfDimensions) {
      throw logic_error("The dimension of the parameter (" + to_string(parameter.n_elem) + ") must match the dimension of the optimisation problem (" + to_string(_numberOfDimensions) + ").");
    }

    return 0;
  }
}
