#pragma once

// C++ standard library
#include <string>
#include <unordered_map>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"
#include "mantella_bits/helper/unorderedContainer.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
namespace mant {
  class RegressionFunction;
}

namespace mant {
  class PolynomialFunctionModel : public SurrogateModel {
   public:
    const std::shared_ptr<RegressionFunction> regressionFunction_;

    explicit PolynomialFunctionModel(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
        const std::shared_ptr<RegressionFunction> regressionFunction);

    std::string toString() const override;

   protected:
    arma::Col<double> coefficients_;

    void modelImplementation() override;

    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const override;
  };
}
