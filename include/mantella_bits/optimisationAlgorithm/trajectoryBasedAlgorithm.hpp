#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm.hpp>

namespace mant {
  template <typename ParameterType>
  class TrajectoryBasedAlgorithm : public OptimisationAlgorithm<ParameterType> {
    public:
      explicit TrajectoryBasedAlgorithm(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      void setInitialParameter(
          const arma::Col<ParameterType>& initialParameter);

    protected:
      arma::Col<ParameterType> initialParameter_;
  };

  template <typename ParameterType>
  TrajectoryBasedAlgorithm<ParameterType>::TrajectoryBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem) {
    // TODO fix for unsigned int
    setInitialParameter(arma::randu<arma::Col<ParameterType>>(OptimisationAlgorithm<ParameterType>::optimisationProblem_->getNumberOfDimensions()) % (OptimisationAlgorithm<ParameterType>::optimisationProblem_->getUpperBounds() - OptimisationAlgorithm<ParameterType>::optimisationProblem_->getLowerBounds()) + OptimisationAlgorithm<ParameterType>::optimisationProblem_->getLowerBounds());
  }

  template <typename ParameterType>
  void TrajectoryBasedAlgorithm<ParameterType>::setInitialParameter(
      const arma::Col<ParameterType>& initialParameter) {
    if(initialParameter.n_elem != OptimisationAlgorithm<ParameterType>::optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the initial parameter (" + std::to_string(initialParameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(OptimisationAlgorithm<ParameterType>::optimisationProblem_->getNumberOfDimensions()) + ").");
    }

    initialParameter_ = initialParameter;
  }
}
