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
    const std::function<double(const arma::Col<double>&)> regressionFunction_;
    const std::function<double(const arma::Col<double>&)> correlationFunction_;

    Kriging(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
        const std::function<double(const arma::Col<double>&)> regressionFunction,
        const std::function<double(const arma::Col<double>&)> correlationFunction);

    void train();
    double predict(
        const arma::Col<double>& parameter) const;

   protected:
    std::unordered_map<arma::Col<double>, double, Hash, IsEqual> samples_;

    arma::Col<double> meanParameter_;
    arma::Col<double> standardDeviationParameter_;

    double meanObjectiveValue_;
    double standardDeviationObjectiveValue_;

    arma::Col<double> beta_;
    arma::Col<double> gamma_;
  };
}
