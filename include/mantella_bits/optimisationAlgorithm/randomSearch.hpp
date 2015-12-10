#pragma once

// C++ standard library
#include <memory>
#include <string>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"

namespace mant {
  class RandomSearch : public OptimisationAlgorithm {
   public:
    explicit RandomSearch();

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& initialParameters) override;

    void optimise(
        OptimisationProblem& optimisationProblem);
  };
}
