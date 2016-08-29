#pragma once

// C++ standard library
#include <string>
#include <memory>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"


namespace mant {
  class Kriging {
   public:
    Kriging(
        const std::function<double(const arma::vec&)> correlationFunction);

    void setCorrelationFunction(
        const std::function<double(const arma::vec&)> correlationFunction);

    const std::function<double(const arma::vec&)> getCorrelationFunction() const;

    void setHighestDegree(
        arma::uword highestDegree);

    arma::uword getHighestDegree() const;

    void train(
        const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples);
    double predict(
        const arma::vec& parameter) const;

   protected:
    std::function<double(const arma::vec&)> correlationFunction_;
    arma::uword highestDegree_;

    std::unordered_map<arma::vec, double, Hash, IsEqual> samples_;

    arma::vec meanParameter_;
    arma::vec standardDeviationParameter_;

    double meanObjectiveValue_;
    double standardDeviationObjectiveValue_;

    arma::vec beta_;
    arma::vec gamma_;

    bool trained_;

    arma::vec polynomialFunction(
        const arma::vec&) const;
  };
}
