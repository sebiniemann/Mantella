#pragma once

// C++ standard library
#include <string>
#include <memory>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/surrogateModel.hpp"
#include "mantella_bits/helper/unorderedContainer.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
namespace mant {
  class CorrelationFunction;
  class RegressionFunction;
}

namespace mant {
  class KrigingModel : public SurrogateModel {
   public:
    const std::shared_ptr<RegressionFunction> regressionFunction_;
    const std::shared_ptr<CorrelationFunction> correlationFunction_;

    KrigingModel(
        const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
        const std::shared_ptr<RegressionFunction> regressionFunction,
        const std::shared_ptr<CorrelationFunction> correlationFunction);

    std::string toString() const override;

   protected:
    arma::Col<double> meanParameter_;
    arma::Col<double> standardDeviationParameter_;

    double meanObjectiveValue_;
    double standardDeviationObjectiveValue_;

    arma::Col<double> beta_;
    arma::Col<double> gamma_;

    void modelImplementation() override;

    double getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const final;
  };
}
