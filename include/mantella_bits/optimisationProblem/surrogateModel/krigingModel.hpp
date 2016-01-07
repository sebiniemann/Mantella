#pragma once

// C++ standard library
#include <functional>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

namespace mant {
  class KrigingModel : public SurrogateModel {
   public:
    KrigingModel(
        const arma::uword numberOfDimensions);

   protected:
    std::function<arma::Col<double>(const arma::Col<double>& parameter)> regressionFunction_;
    std::function<double(const arma::Col<double>& parameter)> correlationFunction_;

    arma::Col<double> meanParameter_;
    arma::Col<double> standardDeviationParameter_;

    double meanObjectiveValue_;
    double standardDeviationObjectiveValue_;

    arma::Col<double> beta_;
    arma::Col<double> gamma_;
  };
}
