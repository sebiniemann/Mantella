#pragma once

// C++ standard library
#include <functional>
#include <string>
#include <unordered_map>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

namespace mant {
  class PolynomialFunctionModel : public SurrogateModel {
   public:
    explicit PolynomialFunctionModel(
        const arma::uword numberOfDimensions);
   
    void setPolynomialOrder(
        const arma::uword polynomialOrder);

    void setEstimatorFunction(
        const std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Row<double>& objectiveValues)> estimatorFunction);

   protected:
    arma::uword polynomialOrder_;
    std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Row<double>& objectiveValues)> estimatorFunction_;
      
    arma::Col<double> coefficients_;
  };
}
