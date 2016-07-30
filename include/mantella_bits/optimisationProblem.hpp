#pragma once

// C++ standard library
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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

    void setObjectiveFunctions(
        const std::vector<std::pair<
            std::function<double(
                const arma::vec& parameter_)>,
            std::string>>& objectiveFunctions);

    std::vector<std::pair<std::function<double(const arma::vec& parameter_)>, std::string>> getObjectiveFunctions() const;

    double getObjectiveValue(
        arma::vec parameter);
    double getObjectiveValueOfNormalisedParameter(
        const arma::vec& normalisedParameter);

    void setLowerBounds(
        const arma::vec& lowerBounds);
    arma::vec getLowerBounds() const;

    void setUpperBounds(
        const arma::vec& upperBounds);
    arma::vec getUpperBounds() const;

    void setParameterPermutation(
        const arma::uvec& parameterPermutation);
    arma::uvec getParameterPermutation() const;

    void setParameterScaling(
        const arma::vec& parameterScaling);
    arma::vec getParameterScaling() const;

    void setParameterTranslation(
        const arma::vec& parameterTranslation);
    arma::vec getParameterTranslation() const;

    void setParameterRotation(
        const arma::mat& parameterRotation);
    arma::mat getParameterRotation() const;

    void setMinimalParameterDistance(
        const arma::vec& minimalParameterDistance);
    arma::vec getMinimalParameterDistance() const;

    void setObjectiveValueScaling(
        const double objectiveValueScaling);
    double getObjectiveValueScaling() const;

    void setObjectiveValueTranslation(
        const double objectiveValueTranslation);
    double getObjectiveValueTranslation() const;

    std::unordered_map<arma::vec, double, Hash, IsEqual> getCachedSamples() const;

    arma::uword getUsedNumberOfEvaluations() const;
    arma::uword getUsedNumberOfDistinctEvaluations() const;

    void reset();

   protected:
    std::vector<std::pair<
        std::function<double(
            const arma::vec& parameter_)>,
        std::string>>
        objectiveFunctions_;

    arma::vec lowerBounds_;
    arma::vec upperBounds_;

    arma::uvec parameterPermutation_;
    arma::vec parameterScaling_;
    arma::vec parameterTranslation_;
    arma::mat parameterRotation_;
    arma::vec minimalParameterDistance_;

    double objectiveValueScaling_;
    double objectiveValueTranslation_;

    std::unordered_map<arma::vec, double, Hash, IsEqual> cachedSamples_;

    arma::uword usedNumberOfEvaluations_;
    arma::uword usedNumberOfDistinctEvaluations_;
  };
}
