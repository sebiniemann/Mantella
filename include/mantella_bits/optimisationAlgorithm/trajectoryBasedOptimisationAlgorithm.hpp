#pragma once

// C++ standard library
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
  class TrajectoryBasedOptimisationAlgorithm : public OptimisationAlgorithm {
   public:
    explicit TrajectoryBasedOptimisationAlgorithm(
        const std::shared_ptr<OptimisationProblem> optimisationProblem);

    void setInitialParameter(
        const arma::Col<double>& initialParameter);

   protected:
    arma::Col<double> initialParameter_;
  };
}
