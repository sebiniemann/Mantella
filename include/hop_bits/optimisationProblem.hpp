#pragma once

#include <cstdlib>
#include <unordered_map>
#include <functional>

#include <armadillo>

#include <cereal/access.hpp>

#include <hop_bits/helper/cereal.hpp>
#include <hop_bits/helper/printable.hpp>

namespace hop {
  class OptimisationProblem : public Printable {
    public:
      explicit OptimisationProblem(const unsigned int& numberOfDimensions);

      OptimisationProblem(const OptimisationProblem&) = delete;
      OptimisationProblem& operator=(const OptimisationProblem&) = delete;

      arma::Col<arma::uword> isSatisfyingLowerBounds(const arma::Col<double>& parameter);
      arma::Col<arma::uword> isSatisfyingUpperBounds(const arma::Col<double>& parameter);
      bool isSatisfyingSoftConstraints(const arma::Col<double>& parameter);
      bool isSatisfyingConstraints(const arma::Col<double>& parameter);

      double getSoftConstraintsValue(const arma::Col<double>& parameter);
      double getObjectiveValue(const arma::Col<double>& parameter);

      unsigned int getNumberOfDimensions() const;

      arma::Col<double> getLowerBounds() const;
      void setLowerBounds(const arma::Col<double>& lowerBounds);

      arma::Col<double> getUpperBounds() const;
      void setUpperBounds(const arma::Col<double>& upperBounds);

      void setParameterTranslation(const arma::Col<double>& parameterTranslation);
      void setParameterRotation(const arma::Mat<double>& parameterRotation);
      void setParameterScale(const arma::Col<double>& parameterScale);
      void setObjectiveValueTranslation(const double& objectiveValueTranslation);
      void setObjectiveValueScale(const double& objectiveValueScale);

      double getAcceptableObjectiveValue() const;
      void setAcceptableObjectiveValue(const double& acceptableObjectiveValue);

      unsigned int getMaximalNumberOfEvaluations() const;
      void setMaximalNumberOfEvaluations(const unsigned int& maximalNumberOfEvaluations);

      unsigned int getNumberOfEvaluations() const;
      unsigned int getNumberOfDistinctEvaluations() const;

      void reset();

    protected:
      virtual double getObjectiveValueImplementation(const arma::Col<double>& parameter) const = 0;
      virtual double getSoftConstraintsValueImplementation(const arma::Col<double>& parameter) const;

      unsigned int numberOfDimensions_;

      arma::Col<double> lowerBounds_;
      arma::Col<double> upperBounds_;

      arma::Col<double> parameterTranslation_;
      arma::Mat<double> parameterRotation_;
      arma::Col<double> parameterScale_;

      double objectiveValueTranslation_;
      double objectiveValueScale_;

      double acceptableObjectiveValue_;
      unsigned int maximalNumberOfEvaluations_;

      unsigned int numberOfEvaluations_;
      unsigned int numberOfDistinctEvaluations_;

      arma::Col<double> getScaledCongruentParameter(const arma::Col<double>& parameter) const;

      class Hasher {
        public:
          std::size_t operator() (const arma::Col<double>& key) const {
            std::size_t hashedKey = std::hash<double>()(key.at(0));

            // Adapted from the Boost library (boost::hash_combine)
            for (const double& value : key) {
              hashedKey ^= std::hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
            }

            return hashedKey;
          }
      };

      class KeyEqual {
        public:
          bool operator() (const arma::Col<double>& firstKey, const arma::Col<double>& secondKey) const {
            return arma::all(firstKey == secondKey);
          }
      };

      std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> cachedObjectiveValues_;
      std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> cachedSoftConstraintsValues_;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> cachedIsSatisfyingLowerBounds_;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> cachedIsSatisfyingUpperBounds_;
      std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> cachedIsSatisfyingSoftConstraints_;
      std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> cachedIsSatisfyingConstraints_;

      friend class cereal::access;
      OptimisationProblem() = default;

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
        archive(cereal::make_nvp("maximalNumberOfEvaluations", maximalNumberOfEvaluations_));
      }
  };
}
