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
    const std::function<arma::vec(const arma::vec&)> polynomialFunction_;
    const std::function<double(const arma::vec&)> correlationFunction_;

    Kriging(
        const std::unordered_map<arma::vec, double, Hash, IsEqual>& samples,
        const std::function<arma::vec(const arma::vec&)> polynomialFunction,
        const std::function<double(const arma::vec&)> correlationFunction);

    void train();
    double predict(
        const arma::vec& parameter) const;

   protected:
    std::unordered_map<arma::vec, double, Hash, IsEqual> samples_;

    arma::vec meanParameter_;
    arma::vec standardDeviationParameter_;

    double meanObjectiveValue_;
    double standardDeviationObjectiveValue_;

    arma::vec beta_;
    arma::vec gamma_;
  };
}
