#pragma once

// C++ Standard Library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Cereal
#include <cereal/access.hpp>

// HOP
#include <hop_bits/helper/cache.hpp>
#include <hop_bits/helper/cereal.hpp>
#include <hop_bits/helper/printable.hpp>

namespace hop {
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
  class OptimisationProblem : public Printable {
    public:
      // Constructs an optimisation problem with the given number of dimensions to be optimised.
      explicit OptimisationProblem(
        // The number of dimensions
        const unsigned int& numberOfDimensions);

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      OptimisationProblem(const OptimisationProblem&) = delete;
      OptimisationProblem& operator=(const OptimisationProblem&) = delete;

      // Checks for each dimension whether the parameter is greater or equal the lower bound.
      // Returns 0 for each dimension where the constraint is violated and 1 otherwise.
      // Note: Computes the result only once and caches it afterwards.
      arma::Col<arma::uword> isSatisfyingLowerBounds(
        // The parameter to be tested
        const arma::Col<double>& parameter);

      // Checks for each dimension whether the parameter is lower or equal the upper bound.
      // Returns 0 for each dimension where the constraint is violated and 1 otherwise.
      // Note: Computes the result only once and caches it afterwards.
      arma::Col<arma::uword> isSatisfyingUpperBounds(
        // The parameter to be tested
        const arma::Col<double>& parameter);

      // Checks whether the soft-constraint is non-zero.
      // Returns false if the constraint is violated and true otherwise.
      // Note: Computes the result only once and caches it afterwards.
      bool isSatisfyingSoftConstraints(
        // The parameter to be tested
        const arma::Col<double>& parameter);

      // Checks whether any interval or soft-constraint is violated.
      // Returns false if the constraint is violated and true otherwise.
      // Note: Computes the results only once per constraint and caches them afterwards.
      bool isSatisfyingConstraints(
        // The parameter to be tested
        const arma::Col<double>& parameter);

      // Returns the sum of all soft-constraint values.
      // The value must always be positive and only 0 if no constraint is violated.
      // Note: Computes the result only once and caches it afterwards.
      double getSoftConstraintsValue(
        // The parameter to be evaluated
        const arma::Col<double>& parameter);

      // Returns the objective value to be minimised
      // Better solutions must have a lower objective value.
      // Note: Computes the result only once and caches it afterwards.
      double getObjectiveValue(
        // The parameter to be evaluated
        const arma::Col<double>& parameter);

      // Returns the number of dimensions to be optimised
      unsigned int getNumberOfDimensions() const noexcept;

      // Returns the lower bounds of the search space.
      arma::Col<double> getLowerBounds() const noexcept;

      // Sets the lower bounds of the search space.
      // Note: It is not checked, whether it is strictly greater then the lower bound or not.
      void setLowerBounds(
        // The new lower bounds
        const arma::Col<double>& lowerBounds);

      // Returns the upper bounds of the search space
      arma::Col<double> getUpperBounds() const noexcept;

      // Sets the upper bounds of the search space.
      // Note: It is not checked, whether it is strictly greater then the lower bound or not.
      void setUpperBounds(
        // The new upper bounds
        const arma::Col<double>& upperBounds);

      // Sets the translation (i.e. shift) which is added (not substracted) to the parameter before
      // any other computation.
      // Note: The translation is always based on the same origin and will not add up is multiple
      // set.
      void setParameterTranslation(
        // The new translation of the parameter space
          const arma::Col<double>& parameterTranslation);

      // Sets the scaling which is multiplied to the parameter after its translation.
      // Reflections due to negative values are permitted.
      // Note: The scaling is always based on the same origin and will not add up is multiple set.
      void setParameterScale(
        // The new scaling of the parameter space
        const arma::Col<double>& parameterScale);

      // Sets the rotation which is multiplied to the parameter after the scaling.
      // Note: The rotation is always based on the same origin and will not add up is multiple set.
      void setParameterRotation(
        // The new rotation of the parameter space
        const arma::Mat<double>& parameterRotation);

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

      std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> getCachedObjectiveValues() const;
      std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> getCachedSoftConstraintsValues() const;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, CacheHasher, CacheKeyEqual> getCachedIsSatisfyingLowerBounds() const;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, CacheHasher, CacheKeyEqual> getCachedIsSatisfyingUpperBounds() const;
      std::unordered_map<arma::Col<double>, bool, CacheHasher, CacheKeyEqual> getCachedIsSatisfyingSoftConstraints() const;
      std::unordered_map<arma::Col<double>, bool, CacheHasher, CacheKeyEqual> getCachedIsSatisfyingConstraints() const;

      // Provides a default deconstructor.
      virtual ~OptimisationProblem() = default;

    protected:
      // The number of dimensions to be optimised
      unsigned int numberOfDimensions_;

      // The lower bound of tthe search space
      arma::Col<double> lowerBounds_;
      // The upper bound of the search space
      arma::Col<double> upperBounds_;

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

      // Actual implementaion of the objective function.
      // Note: The dimension of the parameter is checked beforehand.
      virtual double getObjectiveValueImplementation(
        // The parameter to be evaluated
        const arma::Col<double>& parameter) const = 0;

      // Actual implementaion of the soft-constraints function.
      // Returns 0.0 if not overloaded.
      virtual double getSoftConstraintsValueImplementation(
        // The parameter to be evaluated
        const arma::Col<double>& parameter) const;

      // Returns the rotated, scaled and translated (shifted) parameter.
      arma::Col<double> getScaledCongruentParameter(
        // The parameter to be rotated, scaled and translated (shifted)
        const arma::Col<double>& parameter) const;

      // Several caches used to avoid redundant computations.
      std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> cachedObjectiveValues_;
      std::unordered_map<arma::Col<double>, double, CacheHasher, CacheKeyEqual> cachedSoftConstraintsValues_;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, CacheHasher, CacheKeyEqual> cachedIsSatisfyingLowerBounds_;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, CacheHasher, CacheKeyEqual> cachedIsSatisfyingUpperBounds_;
      std::unordered_map<arma::Col<double>, bool, CacheHasher, CacheKeyEqual> cachedIsSatisfyingSoftConstraints_;
      std::unordered_map<arma::Col<double>, bool, CacheHasher, CacheKeyEqual> cachedIsSatisfyingConstraints_;

      //! The following is ONLY TO BE USED BY CEREAL
      // Gives cereal access to otherwise protected constructors, functions and fields.
      friend class cereal::access;
      // Cereal uses a default constructor and will afterwards update the fields directly.
      OptimisationProblem() = default;

      // The fields to be saved (serialised) and their corresponding name.
      // The given name should always match the WHOLE parameter name (expect the suffix "_") to
      // lessen confusion later on.
      template<class T>
      void serialize(T& archive) {
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
}
