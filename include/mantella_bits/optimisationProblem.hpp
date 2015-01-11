#pragma once

// C++ Standard Library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Cereal
#include <cereal/access.hpp>

// Mantella
#include <mantella_bits/helper/cereal.hpp>
#include <mantella_bits/helper/printable.hpp>
#include <mantella_bits/helper/unordered_map.hpp>

namespace mant {
  // Base class of all optimisation problems in this library. Provides access to all usually
  // needed informations.
  //
  // Within this library, optiomisation problems are viewed as contiunous, real-valued minimisation
  // problems with interval and/or soft-constraints. Support for hard-constraints is not yet
  // implemented.
  //
  // The parameter space and objective values can be translated (shifted), rotated and scaled to
  // avoid biases while evaluating optimisation algorithms.
  //
  // Accessing constraints and the objective value makes use of caches to avoid redundant
  // computations.
  template <typename ParameterType>
  class OptimisationProblem : public Printable {
    public:
      // Constructs an optimisation problem with the given number of dimensions to be optimised.
      explicit OptimisationProblem(
        // The number of dimensions
        const unsigned int& numberOfDimensions) noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      OptimisationProblem(const OptimisationProblem&) = delete;
      OptimisationProblem& operator=(const OptimisationProblem&) = delete;

      // Checks for each dimension whether the parameter is greater or equal the lower bound.
      // Returns 0 for each dimension where the constraint is violated and 1 otherwise.
      // Note: Computes the result only once and caches it afterwards.
      arma::Col<unsigned int> isSatisfyingLowerBounds(
        // The parameter to be tested
        const arma::Col<ParameterType>& parameter);

      // Checks for each dimension whether the parameter is lower or equal the upper bound.
      // Returns 0 for each dimension where the constraint is violated and 1 otherwise.
      // Note: Computes the result only once and caches it afterwards.
      arma::Col<unsigned int> isSatisfyingUpperBounds(
        // The parameter to be tested
        const arma::Col<ParameterType>& parameter);

      // Checks whether the soft-constraint is non-zero.
      // Returns false if the constraint is violated and true otherwise.
      // Note: Computes the result only once and caches it afterwards.
      bool isSatisfyingSoftConstraints(
        // The parameter to be tested
        const arma::Col<ParameterType>& parameter);

      // Checks whether any interval or soft-constraint is violated.
      // Returns false if the constraint is violated and true otherwise.
      // Note: Computes the results only once per constraint and caches them afterwards.
      bool isSatisfyingConstraints(
        // The parameter to be tested
        const arma::Col<ParameterType>& parameter);

      // Returns the sum of all soft-constraint values.
      // The value must always be positive and only 0 if no constraint is violated.
      // Note: Computes the result only once and caches it afterwards.
      double getSoftConstraintsValue(
        // The parameter to be evaluated
        const arma::Col<ParameterType>& parameter);

      // Returns the objective value to be minimised
      // Better solutions must have a lower objective value.
      // Note: Computes the result only once and caches it afterwards.
      double getObjectiveValue(
        // The parameter to be evaluated
        const arma::Col<ParameterType>& parameter);

      // Returns the number of dimensions to be optimised
      unsigned int getNumberOfDimensions() const noexcept;

      // Returns the lower bounds of the search space.
      arma::Col<ParameterType> getLowerBounds() const noexcept;

      // Sets the lower bounds of the search space.
      // Note: It is not checked, whether it is strictly greater then the lower bound or not.
      void setLowerBounds(
        // The new lower bounds
        const arma::Col<ParameterType>& lowerBounds);

      // Returns the upper bounds of the search space
      arma::Col<ParameterType> getUpperBounds() const noexcept;

      // Sets the upper bounds of the search space.
      // Note: It is not checked, whether it is strictly greater then the lower bound or not.
      void setUpperBounds(
        // The new upper bounds
        const arma::Col<ParameterType>& upperBounds);

      // Sets the translation (i.e. shift) which is added (not substracted) to the parameter before
      // any other computation.
      // Note: The translation is always based on the same origin and will not add up is multiple
      // set.
      void setParameterTranslation(
        // The new translation of the parameter space
          const arma::Col<ParameterType>& parameterTranslation);

      // Sets the scaling which is multiplied to the parameter after its translation.
      // Reflections due to negative values are permitted.
      // Note: The scaling is always based on the same origin and will not add up is multiple set.
      void setParameterScale(
        // The new scaling of the parameter space
        const arma::Col<ParameterType>& parameterScale);

      // Sets the rotation which is multiplied to the parameter after the scaling.
      // Note: The rotation is always based on the same origin and will not add up is multiple set.
      void setParameterRotation(
        // The new rotation of the parameter space
        const arma::Mat<ParameterType>& parameterRotation);

      // Sets the translation (i.e. shift) which is added to the objective value after its
      // computation.
      // Note: The translation is always based on the same origin and will not add up is multiple
      // set.
      void setObjectiveValueTranslation(
        // The new translation of the objective value space
        const double& objectiveValueTranslation) noexcept;

      // Sets the scaling which is multiplied to the objective value after its translation.
      // Note: The scaling is always based on the same origin and will not add up is multiple set.
      void setObjectiveValueScale(
        // The new scaling of the objective value space
        const double& objectiveValueScale) noexcept;

      // Returns the upper limit for an objective value to be acceptable. The optimisation process
      // is usually finished after reaching this threshold.
      // Note: The objective value must not be reachable.
      double getAcceptableObjectiveValue() const noexcept;

      // Sets the upper limit for an objective value to be acceptable. The optimisation process
      // is usually finished after reaching this threshold.
      // Note: The objective value must not be reachable.
      void setAcceptableObjectiveValue(
          const double& acceptableObjectiveValue) noexcept;

      // Returns the number of objective function evaluations.
      // Note: The value includes retrival from cache and actual computation.
      unsigned int getNumberOfEvaluations() const noexcept;

      // Returns the number of unique objective function evaluations.
      // Note: This will only include actual computations and no cache retrival.
      unsigned int getNumberOfDistinctEvaluations() const noexcept;

      // Resets the counter by setting the number of (distinct) function evaluations to zero and
      // clearing the caches.
      void reset() noexcept;

      std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> getCachedObjectiveValues() const;
      std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> getCachedSoftConstraintsValues() const;
      std::unordered_map<arma::Col<ParameterType>, arma::Col<unsigned int>, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> getCachedIsSatisfyingLowerBounds() const;
      std::unordered_map<arma::Col<ParameterType>, arma::Col<unsigned int>, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> getCachedIsSatisfyingUpperBounds() const;
      std::unordered_map<arma::Col<ParameterType>, bool, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> getCachedIsSatisfyingSoftConstraints() const;
      std::unordered_map<arma::Col<ParameterType>, bool, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> getCachedIsSatisfyingConstraints() const;

      // Provides a default deconstructor.
      virtual ~OptimisationProblem() = default;

    protected:
      // The number of dimensions to be optimised
      unsigned int numberOfDimensions_;

      // The lower bound of tthe search space
      arma::Col<ParameterType> lowerBounds_;
      // The upper bound of the search space
      arma::Col<ParameterType> upperBounds_;

      // The translation (shift) of the parameter space
      arma::Col<double> parameterTranslation_;
      // The scaling of the parameter space
      arma::Col<double> parameterScale_;
      // The rotation of the parameter space
      arma::Mat<double> parameterRotation_;

      // The translation (shift) of the objective value
      double objectiveValueTranslation_;
      // The scaling of the objective value
      double objectiveValueScale_;

      // The upper limit for an objective value to be acceptable.
      double acceptableObjectiveValue_;

      // The number of objective function evaluations so far (retrived from cache or computated).
      unsigned int numberOfEvaluations_;
      // The number of distinct objective function evaluations so far (computated only).
      unsigned int numberOfDistinctEvaluations_;

      // Returns the rotated, scaled and translated (shifted) parameter.
      arma::Col<ParameterType> getScaledCongruentParameter(
        // The parameter to be rotated, scaled and translated (shifted)
        const arma::Col<ParameterType>& parameter) const noexcept;

      // Actual implementaion of the soft-constraints function.
      // Returns 0.0 if not overloaded.
      virtual double getSoftConstraintsValueImplementation(
        // The parameter to be evaluated
        const arma::Col<ParameterType>& parameter) const noexcept;

      // Actual implementaion of the objective function.
      // Note: the number of dimensions of the parameter is checked beforehand.
      virtual double getObjectiveValueImplementation(
        // The parameter to be evaluated
        const arma::Col<ParameterType>& parameter) const noexcept = 0;

      // Several caches used to avoid redundant computations.
      std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> cachedObjectiveValues_;
      std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> cachedSoftConstraintsValues_;
      std::unordered_map<arma::Col<ParameterType>, arma::Col<unsigned int>, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> cachedIsSatisfyingLowerBounds_;
      std::unordered_map<arma::Col<ParameterType>, arma::Col<unsigned int>, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> cachedIsSatisfyingUpperBounds_;
      std::unordered_map<arma::Col<ParameterType>, bool, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> cachedIsSatisfyingSoftConstraints_;
      std::unordered_map<arma::Col<ParameterType>, bool, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> cachedIsSatisfyingConstraints_;

      // Gives cereal access to otherwise protected constructors, functions and fields.
      friend class cereal::access;
      // Cereal uses a default constructor and will afterwards update the fields directly.
      OptimisationProblem() = default;

      // The fields to be saved (serialised) and their corresponding name.
      // The given name should always match the WHOLE parameter name (expect the suffix "_") to
      // lessen confusion later on.
      template <typename Archive>
      void serialize(
          Archive& archive) noexcept {
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("lowerBounds", lowerBounds_));
        archive(cereal::make_nvp("upperBounds", upperBounds_));
        archive(cereal::make_nvp("parameterTranslation", parameterTranslation_));
        archive(cereal::make_nvp("parameterRotation", parameterRotation_));
        archive(cereal::make_nvp("parameterScale", parameterScale_));
        archive(cereal::make_nvp("objectiveValueTranslation", objectiveValueTranslation_));
        archive(cereal::make_nvp("objectiveValueScale", objectiveValueScale_));
        archive(cereal::make_nvp("acceptableObjectiveValue", acceptableObjectiveValue_));
      }
  };

  template <>
  OptimisationProblem<double>::OptimisationProblem(
      const unsigned int& numberOfDimensions) noexcept;

  template <>
  void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double>& parameterTranslation);

  template <>
  void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double>& parameterRotation);

  template <>
  void OptimisationProblem<double>::setParameterScale(
      const arma::Col<double>& parameterScale);

  template <>
  arma::Col<double> OptimisationProblem<double>::getScaledCongruentParameter(
      const arma::Col<double>& parameter) const noexcept;

  template <typename ParameterType>
  OptimisationProblem<ParameterType>::OptimisationProblem(
      const unsigned int& numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    setLowerBounds(arma::zeros<arma::Col<ParameterType>>(numberOfDimensions_) - std::numeric_limits<ParameterType>::max());
    setUpperBounds(arma::zeros<arma::Col<ParameterType>>(numberOfDimensions_) + std::numeric_limits<ParameterType>::max());
    setObjectiveValueTranslation(0.0);
    setObjectiveValueScale(1.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationProblem<ParameterType>::isSatisfyingLowerBounds(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingLowerBounds_.find(parameter);
    if (cachePosition == cachedIsSatisfyingLowerBounds_.end()) {
      // The result was not found, compute it.
      const arma::Col<unsigned int>& result = (parameter >= lowerBounds_);
      cachedIsSatisfyingLowerBounds_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationProblem<ParameterType>::isSatisfyingUpperBounds(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Check if the result is already cached.
    const auto& cachePosition = cachedIsSatisfyingUpperBounds_.find(parameter);
    if (cachePosition == cachedIsSatisfyingUpperBounds_.end()) {
      // The result was not found, compute it.
      const arma::Col<unsigned int>& result = (parameter <= upperBounds_);
      cachedIsSatisfyingUpperBounds_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <typename ParameterType>
  bool OptimisationProblem<ParameterType>::isSatisfyingSoftConstraints(
      const arma::Col<ParameterType>& parameter) {
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

  template <typename ParameterType>
  bool OptimisationProblem<ParameterType>::isSatisfyingConstraints(
      const arma::Col<ParameterType>& parameter) {
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

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getSoftConstraintsValue(
      const arma::Col<ParameterType>& parameter) {
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

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getObjectiveValue(
      const arma::Col<ParameterType>& parameter) {
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
      const double& result = objectiveValueScale_ * getObjectiveValueImplementation(parameter) + objectiveValueTranslation_;
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <typename ParameterType>
  unsigned int OptimisationProblem<ParameterType>::getNumberOfDimensions() const noexcept {
    return numberOfDimensions_;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationProblem<ParameterType>::getLowerBounds() const noexcept {
    return lowerBounds_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setLowerBounds(
      const arma::Col<ParameterType>& lowerBounds) {
    if (lowerBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the lower bound (" + std::to_string(lowerBounds.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    lowerBounds_ = lowerBounds;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationProblem<ParameterType>::getUpperBounds() const noexcept {
    return upperBounds_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setUpperBounds(
      const arma::Col<ParameterType>& upperBounds) {
    if (upperBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the upper bound (" + std::to_string(upperBounds.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    upperBounds_ = upperBounds;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setObjectiveValueTranslation(
      const double& objectiveValueTranslation) noexcept {
    objectiveValueTranslation_ = objectiveValueTranslation;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::OptimisationProblem::setObjectiveValueScale(
      const double& objectiveValueScale) noexcept {
    objectiveValueScale_ = objectiveValueScale;
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getAcceptableObjectiveValue() const noexcept {
    return acceptableObjectiveValue_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setAcceptableObjectiveValue(
      const double& acceptableObjectiveValue) noexcept {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  template <typename ParameterType>
  unsigned int OptimisationProblem<ParameterType>::getNumberOfEvaluations() const noexcept {
    return numberOfEvaluations_;
  }

  template <typename ParameterType>
  unsigned int OptimisationProblem<ParameterType>::getNumberOfDistinctEvaluations() const noexcept {
    return numberOfDistinctEvaluations_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::reset() noexcept {
    numberOfEvaluations_ = 0;
    numberOfDistinctEvaluations_ = 0;

    cachedObjectiveValues_.clear();
    cachedSoftConstraintsValues_.clear();
    cachedIsSatisfyingLowerBounds_.clear();
    cachedIsSatisfyingUpperBounds_.clear();
    cachedIsSatisfyingSoftConstraints_.clear();
    cachedIsSatisfyingConstraints_.clear();
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> OptimisationProblem<ParameterType>::getCachedObjectiveValues() const {
    return cachedObjectiveValues_;
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> OptimisationProblem<ParameterType>::getCachedSoftConstraintsValues() const {
    return cachedSoftConstraintsValues_;
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, arma::Col<unsigned int>, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> OptimisationProblem<ParameterType>::getCachedIsSatisfyingLowerBounds() const {
    return cachedIsSatisfyingLowerBounds_;
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, arma::Col<unsigned int>, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> OptimisationProblem<ParameterType>::getCachedIsSatisfyingUpperBounds() const {
    return cachedIsSatisfyingUpperBounds_;
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, bool, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> OptimisationProblem<ParameterType>::getCachedIsSatisfyingSoftConstraints() const {
    return cachedIsSatisfyingSoftConstraints_;
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, bool, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>> OptimisationProblem<ParameterType>::getCachedIsSatisfyingConstraints() const {
    return cachedIsSatisfyingConstraints_;
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getSoftConstraintsValueImplementation(
      const arma::Col<ParameterType>& parameter) const noexcept {
    return 0.0;
  }
}
