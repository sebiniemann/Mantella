#pragma once

#include <memory>

#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class OptimisationAlgorithm {
    public:
      explicit OptimisationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      OptimisationAlgorithm(const OptimisationAlgorithm&) = delete;
      OptimisationAlgorithm& operator=(const OptimisationAlgorithm&) = delete;

      void optimise();

      arma::Col<double> getBestSolution() const;
      double getBestObjectiveValue() const;

      bool isFinished() const;
      bool isTerminated() const;

      unsigned int getNumberOfIterations() const;
      void setMaximalNumberOfIterations(const unsigned int& maximalNumberOfIterations);

    protected:
      std::shared_ptr<OptimisationProblem> optimisationProblem_;

      double bestObjectiveValue_;
      arma::Col<double> bestSolution_;

      unsigned int numberOfIterations_;
      unsigned int maximalNumberOfIterations_;

      virtual void optimiseImplementation() = 0;
  };
}
