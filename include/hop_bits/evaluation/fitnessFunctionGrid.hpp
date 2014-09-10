#pragma once

#include <memory>

#include <armadillo>

#include <hop_bits/evaluation.hpp>
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class FitnessFunctionGrid : public Evaluation {
    public:
      explicit FitnessFunctionGrid(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      FitnessFunctionGrid(const FitnessFunctionGrid&) = delete;
      FitnessFunctionGrid& operator=(const FitnessFunctionGrid&) = delete;

      void setLowerBounds(const arma::Col<double>& lowerBounds);
      void setUpperBounds(const arma::Col<double>& upperBounds);

    protected:
      std::shared_ptr<OptimisationProblem> optimisationProblem_;
      arma::Col<double> lowerBounds_;
      arma::Col<double> upperBounds_;

      void runImpelementation();
  };
}
