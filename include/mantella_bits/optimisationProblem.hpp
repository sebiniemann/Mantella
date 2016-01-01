#pragma once
#include "mantella_bits/config.hpp" // IWYU pragma: keep

// C++ standard library
#include <functional>
#include <unordered_map>
#if defined(SUPPORT_MPI) // IWYU pragma: keep
#include <vector>
#endif

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual

namespace mant {
  class OptimisationProblem {
   public:
    const arma::uword numberOfDimensions_;

    explicit OptimisationProblem(
        const arma::uword numberOfDimensions);

    // Objective
    void setObjectiveFunction(
        const std::function<double(const arma::Col<double>& parameter)> objectiveFunction,
        const std::string& objectiveFunctionName);
    void setObjectiveFunction(
        const std::function<double(const arma::Col<double>& parameter)> objectiveFunction);
    std::string getObjectiveFunctionName() const;
    double getObjectiveValue(
        const arma::Col<double>& parameter);
    double getNormalisedObjectiveValue(
        const arma::Col<double>& parameter);

    // Constraints
    void setLowerBounds(
        const arma::Col<double>& lowerBounds);
    arma::Col<double> getLowerBounds() const;
    void setUpperBounds(
        const arma::Col<double>& upperBounds);
    arma::Col<double> getUpperBounds() const;

    // Parameter space modifiers
    void setParameterPermutation(
        const arma::Col<arma::uword>& parameterPermutation);
    arma::Col<arma::uword> getParameterPermutation() const;
    void setParameterScaling(
        const arma::Col<double>& parameterScaling);
    arma::Col<double> getParameterScaling() const;
    void setParameterTranslation(
        const arma::Col<double>& parameterTranslation);
    arma::Col<double> getParameterTranslation() const;
    void setParameterRotation(
        const arma::Mat<double>& parameterRotation);
    arma::Mat<double> getParameterRotation() const;

    // Objective value space modifiers
    void setObjectiveValueScaling(
        const double objectiveValueScaling);
    double getObjectiveValueScaling() const;
    void setObjectiveValueTranslation(
        const double objectiveValueTranslation);
    double getObjectiveValueTranslation() const;

    // Caching
    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> getCachedSamples() const;
    void setMinimalParameterDistance(
        const arma::Col<double>& minimalParameterDistance);
    arma::Col<double> getMinimalParameterDistance() const;

    // Evaluation
    arma::uword getNumberOfEvaluations() const;
    arma::uword getNumberOfDistinctEvaluations() const;
    void reset();

   protected:
    std::function<double(const arma::Col<double>&)> objectiveFunction_;
    std::string objectiveFunctionName_;

    arma::Col<double> lowerBounds_;
    arma::Col<double> upperBounds_;
    arma::Col<double> boundsNormalisiation_;

    arma::Col<arma::uword> parameterPermutation_;
    arma::Col<double> parameterScaling_;
    arma::Col<double> parameterTranslation_;
    arma::Mat<double> parameterRotation_;

    double objectiveValueScaling_;
    double objectiveValueTranslation_;

    double acceptableObjectiveValueThreshold_;

    arma::uword numberOfEvaluations_;
    arma::uword numberOfDistinctEvaluations_;

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> cachedSamples_;
    arma::Col<double> minimalParameterDistance_;

    arma::Col<double> getDiscretisedParameter(
        const arma::Col<double>& parameter) const;
    arma::Col<double> getModifiedParameter(
        const arma::Col<double>& parameter) const;
    double getModifiedObjectiveValue(
        const double objectiveValue) const;
  };
}
