#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
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

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

  template <typename ParameterType>
  TrajectoryBasedAlgorithm<ParameterType>::TrajectoryBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem) {
    // TODO fix for arma::uword
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

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
