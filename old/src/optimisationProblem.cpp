#include "mantella_bits/optimisationProblem.hpp"

// C++ standard library
#include <cassert>
#include <cmath>
#include <utility>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/config.hpp"
#include "mantella_bits/mpi.hpp"

namespace mant {
  OptimisationProblem::OptimisationProblem(
      const arma::uword numberOfDimensions)
      : numberOfDimensions_(synchronise(numberOfDimensions)) {
    assert(numberOfDimensions_ > 0 && "OptimisationProblem: The number of dimensions must be greater than 0.");

    // Initialises the counters.
    reset();

    // Sets default bounds.
    setLowerBounds(arma::zeros<arma::vec>(numberOfDimensions_) - 10);
    setUpperBounds(arma::zeros<arma::vec>(numberOfDimensions_) + 10);

    // Sets all parameter and objective value space modifiers with their neutral element.
    setParameterPermutation(arma::regspace<arma::uvec>(0, numberOfDimensions_ - 1));
    setParameterScaling(arma::ones<arma::vec>(numberOfDimensions_));
    setParameterTranslation(arma::zeros<arma::vec>(numberOfDimensions_));
    if (numberOfDimensions_ > 1) {
      setParameterRotation(arma::eye<arma::mat>(numberOfDimensions_, numberOfDimensions_));
    }
    setObjectiveValueScaling(1.0);
    setObjectiveValueTranslation(0.0);
  }

  void OptimisationProblem::setObjectiveFunctions(
      const std::vector<std::pair<
          std::function<double(
              const arma::vec& parameter_)>,
          std::string>>& objectiveFunctions) {
    assert(objectiveFunctions.size() > 0 && "OptimisationProblem.setObjectiveFunctions: At least one objective function must be defined.");
    for (const auto& objectiveFunction : objectiveFunctions) {
      assert(static_cast<bool>(objectiveFunction.first) && "OptimisationProblem.setObjectiveFunctions: All objective functions must be callable.");
    }

    objectiveFunctions_ = objectiveFunctions;

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  std::vector<std::pair<std::function<double(const arma::vec& parameter_)>, std::string>> OptimisationProblem::getObjectiveFunctions() const {
    return objectiveFunctions_;
  }

  double OptimisationProblem::getObjectiveValue(
      arma::vec parameter) {
    assert(parameter.n_elem == numberOfDimensions_ && "OptimisationProblem.getObjectiveValue: The number of elements must be equal to the optimisation problem's number of dimensions.");
    assert(!parameter.has_nan() && "OptimisationProblem.getObjectiveValue: The parameter must not have NaNs.");
    assert(objectiveFunctions_.size() > 0 && "OptimisationProblem.getObjectiveValue: At least one objective function must be defined.");

    ++usedNumberOfEvaluations_;

    // Permutations are only affective for at least 2 dimensional problems.
    if (numberOfDimensions_ > 1) {
      parameter = parameter.elem(parameterPermutation_);
    }
    parameter = (parameterScaling_ % parameter - parameterTranslation_);
    // Rotations are only defined for at least 2 dimensional problems.
    if (numberOfDimensions_ > 1) {
      parameter = parameterRotation_ * parameter;
    }

    double objectiveValue = 0.0;
    for (const auto& objectiveFunction : objectiveFunctions_) {
      objectiveValue += objectiveFunction.first(parameter);
      assert(!std::isnan(objectiveValue));
    }

    return objectiveValueScaling_ * objectiveValue + objectiveValueTranslation_;
  }

  double OptimisationProblem::getObjectiveValueOfNormalisedParameter(
      const arma::vec& normalisedParameter) {
    assert(normalisedParameter.n_elem == numberOfDimensions_ && "OptimisationProblem.getObjectiveValueOfNormalisedParameter: The number of elements must be equal to the optimisation problem's number of dimensions.");
    assert(arma::all(lowerBounds_ <= upperBounds_) && "OptimisationProblem.getObjectiveValueOfNormalisedParameter: All upper bounds must be greater than their lower one.");

    return getObjectiveValue(lowerBounds_ + normalisedParameter % (upperBounds_ - lowerBounds_));
  }

  void OptimisationProblem::setLowerBounds(
      const arma::vec& lowerBounds) {
    assert(lowerBounds.n_elem == numberOfDimensions_ && "OptimisationProblem.setLowerBounds: The lower bounds' number of elements must be equal to the optimisation problem's number of dimensions.");

    lowerBounds_ = synchronise(lowerBounds);
  }

  arma::vec OptimisationProblem::getLowerBounds() const {
    return lowerBounds_;
  }

  void OptimisationProblem::setUpperBounds(
      const arma::vec& upperBounds) {
    assert(upperBounds.n_elem == numberOfDimensions_ && "OptimisationProblem.setUpperBounds: The upper bounds' number of elements must be equal to the optimisation problem's number of dimensions.");

    upperBounds_ = synchronise(upperBounds);
  }

  arma::vec OptimisationProblem::getUpperBounds() const {
    return upperBounds_;
  }

  void OptimisationProblem::setParameterPermutation(
      const arma::uvec& parameterPermutation) {
    assert(parameterPermutation.n_elem == numberOfDimensions_ && "OptimisationProblem.setParameterPermutation: The parameter permutation's number of elements must be equal to the optimisation problem's number of dimensions.");
    assert(isPermutationVector(parameterPermutation, numberOfDimensions_, numberOfDimensions_) && "OptimisationProblem.setParameterPermutation: The parameter permutation must be an actual permutation on the optimisation problem.");

    parameterPermutation_ = synchronise(parameterPermutation);

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::uvec OptimisationProblem::getParameterPermutation() const {
    return parameterPermutation_;
  }

  void OptimisationProblem::setParameterTranslation(
      const arma::vec& parameterTranslation) {
    assert(parameterTranslation.n_elem != numberOfDimensions_ && "OptimisationProblem.setParameterTranslation: The parameter translation's number of elements must be equal to the optimisation problem's number of dimensions.");

    parameterTranslation_ = synchronise(parameterTranslation);

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::vec OptimisationProblem::getParameterTranslation() const {
    return parameterTranslation_;
  }

  void OptimisationProblem::setParameterScaling(
      const arma::vec& parameterScaling) {
    assert(parameterScaling.n_elem == numberOfDimensions_ && "OptimisationProblem.setParameterScaling: The parameter scaling's number of elements must be equal to the optimisation problem's number of dimensions.");

    parameterScaling_ = synchronise(parameterScaling);

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::vec OptimisationProblem::getParameterScaling() const {
    return parameterScaling_;
  }

  void OptimisationProblem::setParameterRotation(
      const arma::mat& parameterRotation) {
    assert(parameterRotation.n_rows == numberOfDimensions_ && "OptimisationProblem.setParameterRotation: The parameter rotation's number of rows must be equal to the optimisation problem's number of dimensions.");
    assert(isRotationMatrix(parameterRotation) && "OptimisationProblem.setParameterRotation: The parameter rotation must be an actual rotation matrix.");

    parameterRotation_ = synchronise(parameterRotation);

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  arma::mat OptimisationProblem::getParameterRotation() const {
    return parameterRotation_;
  }

  void OptimisationProblem::setObjectiveValueTranslation(
      const double objectiveValueTranslation) {
    objectiveValueTranslation_ = synchronise(objectiveValueTranslation);

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  void OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) {
    objectiveValueScaling_ = synchronise(objectiveValueScaling);

    // Resets all counters and caches, as the problem could have changed.
    reset();
  }

  double OptimisationProblem::getObjectiveValueScaling() const {
    return objectiveValueScaling_;
  }

  double OptimisationProblem::getObjectiveValueTranslation() const {
    return objectiveValueTranslation_;
  }

  arma::uword OptimisationProblem::getUsedNumberOfEvaluations() const {
    return usedNumberOfEvaluations_;
  }

  void OptimisationProblem::reset() {
    usedNumberOfEvaluations_ = 0;
  }
}
