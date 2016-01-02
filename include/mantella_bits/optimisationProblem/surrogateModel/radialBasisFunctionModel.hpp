#pragma once

// C++ standard library
#include <functional>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"

namespace mant {
  class RadialBasisFunctionModel : public SurrogateModel {
   public:
    explicit RadialBasisFunctionModel(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples);

   protected:
    std::function<double(const double value)> radialBasisFunction_;

    arma::Col<double> coefficients_;

    void modelImplementation() override;

    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override;
  };
}
