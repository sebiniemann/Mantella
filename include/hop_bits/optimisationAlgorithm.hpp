#pragma once

// C++ STL
#include <memory>

// HOP
#include <hop_bits/optimisationProblem.hpp>
#include <hop_bits/helper/printable.hpp>

namespace hop {
  class OptimisationAlgorithm : public Printable {
    public:
      // Constructs an optimisation algorithm with the given problem to be optimised.
      explicit OptimisationAlgorithm(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      OptimisationAlgorithm(const OptimisationAlgorithm&) = delete;
      OptimisationAlgorithm& operator=(const OptimisationAlgorithm&) = delete;

      // Starts the optimisation process.
      // Note: The best parameter and its objective value can be retrived via the corresponding
      // getter, after the optimisation process is finished or terminates.
      void optimise();

      // Returns the best solution (parameter) yet found.
      arma::Col<double> getBestSolution() const;
      // Returns the objective value of the best solution (parameter) yet found.
      double getBestObjectiveValue() const;
      // Checks whether the currently best objective value satisfies the upper bound for an
      // acceptable objective value.
      // Returns true if the objective value is lower of equal the upper bound and false otherwise.
      bool isFinished() const;

      // Checks whether the current number of iterations performed violates the maximal number of
      // iterations.
      // Returns true if the number of iterations is strictly lower the maximal number of iterations
      // and false otherwise.
      bool isTerminated() const;

      // Returns the current number of iterations performed.
      unsigned int getNumberOfIterations() const;

      // Sets the maximal number of iterrations to be performed.
      // The optimisation process will terminate after reaching this limit.
      void setMaximalNumberOfIterations(const unsigned int& maximalNumberOfIterations);

      // Provides a default deconstructor.
      ~OptimisationAlgorithm() = default;

    protected:
      // The optimisation problem to be optimised.
      std::shared_ptr<OptimisationProblem> optimisationProblem_;

      // The best solution (parameter) yet found.
      // Note: The best parameter is always selected in the following order. If any criteria is not
      // fulfilled for any parameter yet found, the best one given for the remaining parameters is
      // selected.
      // 1 - Fulfills the interval-constraints
      // 2 - Has the lowest soft-constraint value yet found
      // 3 - Has the lowest objective value yet found
      arma::Col<double> bestSolution_;
      // The objective value of the best solution (parameter) yet found.
      double bestObjectiveValue_;

      // The current number of iterations performed.
      unsigned int numberOfIterations_;
      // The maximal number of iterations to be performed.
      unsigned int maximalNumberOfIterations_;

      // The actual optimisation procedere.
      // Note: The counter within the optimisation problem (counting the number of distinct function
      // evaluations for example) are already reset beforehand.
      virtual void optimiseImplementation() = 0;

      // Resets the counter by setting the number of iterations to 0, the best solution (parameter)
      // to "not a number" and the corresponding objective value to infinity.
      // Note: This will also call the reset funtion of the optimisation problem.
      void reset();
  };
}
