#pragma once

// C++ STL
#include <cstdlib>
#include <unordered_map>
#include <functional>

// Armadillo
#include <armadillo>

// Cereal
#include <cereal/access.hpp>

// HOP
#include <hop_bits/helper/cereal.hpp>
#include <hop_bits/helper/printable.hpp>

namespace hop {
  // Base class of all optimisation problems in this library. Provides access to all usually
  // needed informations.
  //
  // Within this library, optiomisation problems are viewed as contiunous, real-valued minimisation
  // problems with interval- and/or soft-constraints. Support for hard-constraints is not yet
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
      explicit OptimisationProblem(const unsigned int& numberOfDimensions);

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      OptimisationProblem(const OptimisationProblem&) = delete;
      OptimisationProblem& operator=(const OptimisationProblem&) = delete;

      // Checks for each dimension whether the parameter is greater or equal the lower bound.
      // Returns 0 for each dimension where the constraint is violated and 1 otherwise.
      // Note: Computes the result only once and caches it afterwards.
      arma::Col<arma::uword> isSatisfyingLowerBounds(const arma::Col<double>& parameter);

      // Checks for each dimension whether the parameter is lower or equal the upper bound.
      // Returns 0 for each dimension where the constraint is violated and 1 otherwise.
      // Note: Computes the result only once and caches it afterwards.
      arma::Col<arma::uword> isSatisfyingUpperBounds(const arma::Col<double>& parameter);

      // Checks whether the soft-constraint is non-zero.
      // Returns false if the constraint is violated and true otherwise.
      // Note: Computes the result only once and caches it afterwards.
      bool isSatisfyingSoftConstraints(const arma::Col<double>& parameter);

      // Checks whether any interval- or soft-constraint is violated.
      // Returns false if the constraint is violated and true otherwise.
      // Note: Computes the results only once per constraint and caches them afterwards.
      bool isSatisfyingConstraints(const arma::Col<double>& parameter);

      // Returns the sum of all soft-constraint values.
      // The value must always be positive and only 0 if no constraint is violated.
      // Note: Computes the result only once and caches it afterwards.
      double getSoftConstraintsValue(const arma::Col<double>& parameter);

      // Returns the objective value to be minimised
      // Better solutions must have a lower objective value.
      // Note: Computes the result only once and caches it afterwards.
      double getObjectiveValue(const arma::Col<double>& parameter);

      // Returns the number of dimensions to be optimised.
      unsigned int getNumberOfDimensions() const;

      // Returns the lower bounds of the search space.
      arma::Col<double> getLowerBounds() const;

      // Sets the lower bounds of the search space.
      // This must be strictly lower then the upper bound for each dimension.
      void setLowerBounds(const arma::Col<double>& lowerBounds);

      // Returns the upper bounds of the search space
      arma::Col<double> getUpperBounds() const;

      // Sets the upper bounds of the search space.
      // This must be strictly greater then the lower bound for each dimension.
      void setUpperBounds(const arma::Col<double>& upperBounds);

      // Sets the translation (i.e. shift) which is added to the parameter before any other
      // computation.
      void setParameterTranslation(const arma::Col<double>& parameterTranslation);

      // Sets the scaling which is multiplied to the parameter after its translation.
      void setParameterScale(const arma::Col<double>& parameterScale);

      // Sets the rotation which is multiplied to the parameter after the scaling.
      void setParameterRotation(const arma::Mat<double>& parameterRotation);

      // Sets the translation (i.e. shift) which is added to the objective value after its
      // computation.
      void setObjectiveValueTranslation(const double& objectiveValueTranslation);

      // Sets the scaling which is multiplied to the objective value after its translation.
      void setObjectiveValueScale(const double& objectiveValueScale);

      // Returns the upper limit for an objective value to be acceptable. The optimisation process
      // is usually finished after reaching this threshold.
      // Note: The value must not actual be reachable.
      double getAcceptableObjectiveValue() const;

      // Sets the upper limit for an objective value to be acceptable. The optimisation process
      // is usually finished after reaching this threshold.
      // Note: The value must not actual be reachable.
      void setAcceptableObjectiveValue(const double& acceptableObjectiveValue);

      // Returns the number of objective function evaluations.
      // Note: The value includes retrival from cache and actual computation.
      unsigned int getNumberOfEvaluations() const;

      // Returns the number of unique objective function evaluations.
      // Note: This will only include actual computations and no cache retrival.
      unsigned int getNumberOfDistinctEvaluations() const;

      // Resets the counter by setting the number of (distinct) function evaluations to zero.
      void reset();

      // Provides a default deconstructor.
      virtual ~OptimisationProblem() = default;

    protected:
      // Actual implementaion of the objective function.
      // Note: The dimension of the parameter is checked beforehand.
      virtual double getObjectiveValueImplementation(const arma::Col<double>& parameter) const = 0;
      // Actual implementaion of the soft-constraints function.
      // Returns 0.0 if not overloaded.
      virtual double getSoftConstraintsValueImplementation(const arma::Col<double>& parameter) const;

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

      // Returns the rotated, scaled and translated (shifted) parameter.
      arma::Col<double> getScaledCongruentParameter(const arma::Col<double>& parameter) const;

      //! Helper classes ONLY TO BE USED BY THE CACHE MECHANISM
      // Calculates a hash value for custom types.
      class Hasher {
        public:
          // Returns a hash value for column vectors.
          // Note: This is adapted from the Boost library (boost::hash_combine).
          std::size_t operator() (const arma::Col<double>& key) const {
            // Start with the hash of the first value ...
            std::size_t hashedKey = std::hash<double>()(key.at(0));

            // ... and add the hash value of all following values to it.
            for (const double& value : key) {
              hashedKey ^= std::hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
            }

            return hashedKey;
          }
      };

      // Checks if two values of a custom type are equal.
      class KeyEqual {
        public:
          // Returns true if all values of the provided column vectors are equal.
          bool operator() (const arma::Col<double>& firstKey, const arma::Col<double>& secondKey) const {
            // This will also check if both vectors have the same size.
            return arma::all(firstKey == secondKey);
          }
      };

      // Several caches used to avoid redundant computations.
      std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> cachedObjectiveValues_;
      std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> cachedSoftConstraintsValues_;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> cachedIsSatisfyingLowerBounds_;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> cachedIsSatisfyingUpperBounds_;
      std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> cachedIsSatisfyingSoftConstraints_;
      std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> cachedIsSatisfyingConstraints_;


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
