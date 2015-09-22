#pragma once

// C++ standard library
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/config.hpp>
#include <mantella_bits/helper/printable.hpp>
#include <mantella_bits/helper/unorderedContainer.hpp>

namespace mant {
  class OptimisationProblem : public Printable {
   public:
    const arma::uword numberOfDimensions_;

    explicit OptimisationProblem(
        const arma::uword numberOfDimensions);

    void setLowerBounds(
        const arma::Col<double>& lowerBounds);

    void setUpperBounds(
        const arma::Col<double>& upperBounds);

    arma::Col<double> getLowerBounds() const;

    arma::Col<double> getUpperBounds() const;

    arma::Col<arma::uword> isWithinLowerBounds(
        const arma::Col<double>& parameter);

    arma::Col<arma::uword> isWithinUpperBounds(
        const arma::Col<double>& parameter);

    void setParameterPermutation(
        const arma::Col<arma::uword>& parameterPermutation);

    void setParameterScaling(
        const arma::Col<double>& parameterScaling);

    void setParameterTranslation(
        const arma::Col<double>& parameterTranslation);

    void setParameterRotation(
        const arma::Mat<double>& parameterRotation);

    void setObjectiveValueScaling(
        const double objectiveValueScaling);

    void setObjectiveValueTranslation(
        const double objectiveValueTranslation);

    void setAcceptableObjectiveValue(
        const double acceptableObjectiveValue);

    double getAcceptableObjectiveValue() const;

    double getObjectiveValue(
        const arma::Col<double>& parameter);

    arma::uword getNumberOfEvaluations() const;

    arma::uword getNumberOfDistinctEvaluations() const;

    void reset();

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> getCachedSamples() const;

#if defined(SUPPORT_MPI)
    std::vector<double> serialise() const;

    void deserialise(
        std::vector<double> serialisedOptimisationProblem);
#endif

    virtual ~OptimisationProblem() = default;

   protected:
    arma::Col<double> lowerBounds_;
    arma::Col<double> upperBounds_;

    arma::Col<arma::uword> parameterPermutation_;
    arma::Col<double> parameterScaling_;
    arma::Col<double> parameterTranslation_;
    arma::Mat<double> parameterRotation_;

    double objectiveValueScaling_;
    double objectiveValueTranslation_;

    double acceptableObjectiveValue_;

    arma::uword numberOfEvaluations_;
    arma::uword numberOfDistinctEvaluations_;

    arma::Col<double> getDiversifiedParameter(
        const arma::Col<double>& parameter) const;

    virtual double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const = 0;

    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> cachedSamples_;
  };
}
