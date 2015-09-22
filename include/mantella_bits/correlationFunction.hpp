#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>

namespace mant {
  class CorrelationFunction : public Printable {
   public:
    const arma::uword numberOfDimensions_;

    explicit CorrelationFunction(
        const arma::uword numberOfDimensions);

    void setCorrelationWeights(
        const arma::Col<double>& correlationWeights);

    arma::Col<double> getCorrelationWeight() const;

    double getCorrelationCoefficient(
        const arma::Col<double>& parameter) const;

    virtual ~CorrelationFunction() = default;

   protected:
    arma::Col<double> correlationWeights_;

    virtual double getCorrelationCoefficientImplementation(
        const arma::Col<double>& parameter) const = 0;
  };
}
