#pragma once

#include <cstdlib>
#include <unordered_map>
#include <functional>

#include <armadillo>

#include <cereal/access.hpp>

#include <helper/cereal.hpp>

namespace hop {
  class OptimisationProblem {
    public:
      OptimisationProblem(const unsigned int& numberOfDimensions);

      virtual arma::Col<arma::uword> isSatisfyingLowerBounds(const arma::Col<double>& parameter) final;
      virtual arma::Col<arma::uword> isSatisfyingUpperBounds(const arma::Col<double>& parameter) final;
      virtual bool isSatisfyingSoftConstraints(const arma::Col<double>& parameter) final;
      virtual bool isSatisfyingConstraints(const arma::Col<double>& parameter) final;

      virtual double getSoftConstraintsValue(const arma::Col<double>& parameter) final;
      virtual double getObjectiveValue(const arma::Col<double>& parameter) final;

      virtual unsigned int getNumberOfDimensions() const final;

      virtual arma::Col<double> getLowerBounds() const final;
      virtual void setLowerBounds(const arma::Col<double>& lowerBounds);

      virtual arma::Col<double> getUpperBounds() const final;
      virtual void setUpperBounds(const arma::Col<double>& upperBounds) final;

      virtual void setParameterTranslation(const arma::Col<double>& parameterTranslation) final;
      virtual void setParameterRotation(const arma::Mat<double>& parameterRotation) final;
      virtual void setParameterScale(const arma::Col<double>& parameterScale) final;
      virtual void setObjectiveValueTranslation(const double& objectiveValueTranslation) final;
      virtual void setObjectiveValueScale(const double& objectiveValueScale) final;

      virtual double getAcceptableObjectiveValue() const final;
      virtual void setAcceptableObjectiveValue(const double& acceptableObjectiveValue) final;

      virtual unsigned int getMaximalNumberOfEvaluations() const final;
      virtual void setMaximalNumberOfEvaluations(const unsigned int& maximalNumberOfEvaluations) final;

      virtual unsigned int getNumberOfEvaluations() const final;
      
      virtual ~OptimisationProblem() = default;

    protected:
      virtual double getObjectiveValueImplementation(const arma::Col<double>& parameter) const = 0;
      virtual double getSoftConstraintsValueImplementation(const arma::Col<double>& parameter) const;

      unsigned int _numberOfDimensions;

      arma::Col<double> _lowerBounds;
      arma::Col<double> _upperBounds;

      arma::Col<double> _parameterTranslation;
      arma::Mat<double> _parameterRotation;
      arma::Col<double> _parameterScale;

      double _objectiveValueTranslation;
      double _objectiveValueScale;

      double _acceptableObjectiveValue;
      unsigned int _maximalNumberOfEvaluations;

      unsigned int _numberOfEvaluations;

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

      std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> _cachedObjectivValues;
      std::unordered_map<arma::Col<double>, double, Hasher, KeyEqual> _cachedSoftConstraintsValues;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> _cachedIsSatisfyingLowerBounds;
      std::unordered_map<arma::Col<double>, arma::Col<arma::uword>, Hasher, KeyEqual> _cachedIsSatisfyingUpperBounds;
      std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> _cachedIsSatisfyingSoftConstraints;
      std::unordered_map<arma::Col<double>, bool, Hasher, KeyEqual> _cachedIsSatisfyingConstraints;

      friend class cereal::access;
      OptimisationProblem() = default;

      template<class T>
      void serialize(T& archive) {
        archive(CEREAL_NVP(_numberOfDimensions));
        archive(CEREAL_NVP(_lowerBounds));
        archive(CEREAL_NVP(_upperBounds));
        archive(CEREAL_NVP(_parameterTranslation));
        archive(CEREAL_NVP(_parameterRotation));
        archive(CEREAL_NVP(_parameterScale));
        archive(CEREAL_NVP(_objectiveValueTranslation));
        archive(CEREAL_NVP(_objectiveValueScale));
        archive(CEREAL_NVP(_acceptableObjectiveValue));
        archive(CEREAL_NVP(_maximalNumberOfEvaluations));
      }
  };
}
