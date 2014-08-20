#pragma once

#include <unordered_map>
using std::unordered_map;

#include <limits>
using std::numeric_limits;

#include <cstdlib>
using std::size_t;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <functional>
using std::hash;

#include <armadillo>
using arma::Mat;
using arma::Col;
using arma::uword;
using arma::all;

namespace hop {
  class OptimisationProblem {
    public:
      OptimisationProblem(const unsigned int& numberOfDimensions);

      virtual Col<uword> isSatisfyingLowerBounds(const Col<double>& parameter) final;
      virtual Col<uword> isSatisfyingUpperBounds(const Col<double>& parameter) final;
      virtual bool isSatisfyingSoftConstraints(const Col<double>& parameter) final;
      virtual bool isSatisfyingConstraints(const Col<double>& parameter) final;

      virtual double getSoftConstraintsValue(const Col<double>& parameter) final;
      virtual double getObjectiveValue(const Col<double>& parameter) final;

      virtual unsigned int getNumberOfDimensions() const final;

      virtual Col<double> getLowerBounds() const final;
      virtual void setLowerBounds(const Col<double>& lowerBounds);

      virtual Col<double> getUpperBounds() const final;
      virtual void setUpperBounds(const Col<double>& upperBounds) final;

      virtual void setParameterTranslation(const Col<double>& parameterTranslation) final;
      virtual void setParameterRotation(const Mat<double>& parameterRotation) final;
      virtual void setParameterReflection(const Col<double>& parameterReflection) final;
      virtual void setParameterScale(const Mat<double>& parameterScale) final;
      virtual void setObjectiveValueTranslation(const double& objectiveValueTranslation) final;
      virtual void setObjectiveValueScale(const double& objectiveValueScale) final;

      virtual double getAcceptableObjectiveValue() const final;
      virtual void setAcceptableObjectiveValue(const double& acceptableObjectiveValue) final;

      virtual unsigned int getMaximalNumberOfEvaluations() const final;
      virtual void setMaximalNumberOfEvaluations(const unsigned int& maximalNumberOfEvaluations) final;

      virtual unsigned int getNumberOfEvaluations() const final;
      
      virtual ~OptimisationProblem() = default;

    protected:
      virtual double getObjectiveValueImplementation(const Col<double>& parameter) const = 0;
      virtual double getSoftConstraintsValueImplementation(const Col<double>& parameter) const;

      unsigned int _numberOfDimensions;

      Col<double> _lowerBounds;
      Col<double> _upperBounds;

      Col<double> _parameterTranslation;
      Mat<double> _parameterRotation;
      Col<double> _parameterReflection;
      Mat<double> _parameterScale;

      double _objectiveValueTranslation;
      double _objectiveValueScale;

      double _acceptableObjectiveValue;
      unsigned int _maximalNumberOfEvaluations;

      unsigned int _numberOfEvaluations;

      Col<double> getScaledCongruentParameter(const Col<double>& parameter) const;

      class Hasher {
        public:
          size_t operator() (const Col<double>& key) const {
            size_t hashedKey = hash<double>()(key.at(0));

            // Adapted from the Boost library (boost::hash_combine)
            for (const double& value : key) {
              hashedKey ^= hash<double>()(value) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
            }

            return hashedKey;
          }
      };

      class KeyEqual {
        public:
          bool operator() (const Col<double>& firstKey, const Col<double>& secondKey) const {
            return all(firstKey == secondKey);
          }
      };

      unordered_map<Col<double>, double, Hasher, KeyEqual> _cachedObjectivValues;
      unordered_map<Col<double>, double, Hasher, KeyEqual> _cachedSoftConstraintsValues;
      unordered_map<Col<double>, Col<uword>, Hasher, KeyEqual> _cachedIsSatisfyingLowerBounds;
      unordered_map<Col<double>, Col<uword>, Hasher, KeyEqual> _cachedIsSatisfyingUpperBounds;
      unordered_map<Col<double>, bool, Hasher, KeyEqual> _cachedIsSatisfyingSoftConstraints;
      unordered_map<Col<double>, bool, Hasher, KeyEqual> _cachedIsSatisfyingConstraints;
  };
}
