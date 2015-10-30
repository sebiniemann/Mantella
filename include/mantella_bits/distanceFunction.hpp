#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/helper/printable.hpp>

namespace mant {
  class DistanceFunction : public Printable {
   public:
    const arma::uword numberOfDimensions_;

    explicit DistanceFunction(
        const arma::uword numberOfDimensions);

    double getDistance(
        const arma::Col<double>& firstParameter,
        const arma::Col<double>& secondParameter) const;

    double getLength(
        const arma::Col<double>& parameter) const;

    virtual ~DistanceFunction() = default;

   protected:
    virtual double getLengthImplementation(
        const arma::Col<double>& parameter) const = 0;
  };
}
