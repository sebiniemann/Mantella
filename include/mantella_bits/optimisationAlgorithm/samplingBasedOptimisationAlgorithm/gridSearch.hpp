#pragma once

// C++ standard library
#include <memory>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm/samplingBasedOptimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class GridSearch : public SamplingBasedOptimisationAlgorithm {
   public:
    explicit GridSearch(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setNumberOfSamplesPerDimension(
        const arma::Col<arma::uword>& numberOfSamplesPerDimension);

    arma::Col<arma::uword> getNumberOfSamplesPerDimension() const;

    std::string toString() const override;

   protected:
    arma::Col<arma::uword> numberOfSamplesPerDimension_;

    void optimiseImplementation() override;
  };
}
