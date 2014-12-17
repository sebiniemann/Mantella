#pragma once

// C++ Standard Library
#include <memory>

// HOP
#include <hop_bits/optimisationProblem.hpp>
#include <hop_bits/helper/printable.hpp>

namespace hop {
  template <typename ParameterType>
  class OptimisationAlgorithm : public Printable {
    public:
      // Constructs an optimisation algorithm with the given problem to be optimised.
      explicit OptimisationAlgorithm(
          // The problem to be optimised
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      OptimisationAlgorithm(const OptimisationAlgorithm&) = delete;
      OptimisationAlgorithm& operator=(const OptimisationAlgorithm&) = delete;

      // Starts the optimisation process.
      // Note: The best parameter and its objective value can be retrived via the corresponding
      // getter, after the optimisation process is finished or terminates.
      void optimise() noexcept;

      // Returns the current number of iterations performed.
      unsigned int getNumberOfIterations() const noexcept;

      // Sets the maximal number of iterrations to be performed.
      // The optimisation process will terminate after reaching this limit.
      void setMaximalNumberOfIterations(const unsigned int& maximalNumberOfIterations) noexcept;

      // Returns the best parameter yet found.
      // Note: The best parameter is always selected in the following order.
      // 1 - Fulfills the interval-constraints
      // 2 - Has the lowest soft-constraint value yet found
      // 3 - Has the lowest objective value yet found
      arma::Col<ParameterType> getBestParameter() const noexcept;

      // Returns the objective value of the best parameter yet found.
      double getBestSoftConstraintsValue() const noexcept;

      // Returns the objective value of the best parameter yet found.
      double getBestObjectiveValue() const noexcept;

      // Checks whether the currently best objective value satisfies the upper bound for an
      // acceptable objective value.
      // Returns true if the objective value is lower of equal the upper bound and false otherwise.
      bool isFinished() const noexcept;

      // Checks whether the current number of iterations performed violates the maximal number of
      // iterations.
      // Returns true if the number of iterations is strictly lower the maximal number of iterations
      // and false otherwise.
      bool isTerminated() const noexcept;

      // Provides a default deconstructor.
      virtual ~OptimisationAlgorithm() = default;

    protected:
      // The optimisation problem to be optimised.
      std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem_;

      // The current number of iterations performed.
      unsigned int numberOfIterations_;

      // The maximal number of iterations to be performed.
      unsigned int maximalNumberOfIterations_;

      // The best parameter yet found.
      arma::Col<ParameterType> bestParameter_;

      // The best parameter's soft-constraint value.
      double bestSoftConstraintsValue_;

      // The best parameter's objective value.
      double bestObjectiveValue_;

      // The actual optimisation procedere.
      // Note: The counter within the optimisation problem (counting the number of distinct function
      // evaluations for example) are already reset beforehand.
      virtual void optimiseImplementation() = 0;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  OptimisationAlgorithm<ParameterType>::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : optimisationProblem_(optimisationProblem),
      numberOfIterations_(0),
      bestSoftConstraintsValue_(std::numeric_limits<double>::infinity()),
      bestObjectiveValue_(std::numeric_limits<double>::infinity()) {
    setMaximalNumberOfIterations(1000);
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::optimise() noexcept {
    // Reset results, counters and caches
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestSoftConstraintsValue_ = std::numeric_limits<double>::infinity();
    bestParameter_.reset();
    numberOfIterations_ = 0;
    optimisationProblem_->reset();

    return optimiseImplementation();
  }

  template <typename ParameterType>
  unsigned int OptimisationAlgorithm<ParameterType>::getNumberOfIterations() const noexcept {
    return numberOfIterations_;
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::setMaximalNumberOfIterations(
      const unsigned int& maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationAlgorithm<ParameterType>::getBestParameter() const noexcept {
    return bestParameter_;
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getBestSoftConstraintsValue() const noexcept {
    return bestSoftConstraintsValue_;
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getBestObjectiveValue() const noexcept {
    return bestObjectiveValue_;
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isFinished() const noexcept {
    return (bestSoftConstraintsValue_ == 0.0 && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  //! Only PROTECTED or PRIVATE methods from here on
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
