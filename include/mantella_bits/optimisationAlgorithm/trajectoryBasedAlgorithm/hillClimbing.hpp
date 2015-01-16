#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class HillClimbing : public TrajectoryBasedAlgorithm<ParameterType, DistanceFunction> {
    public:
      explicit HillClimbing(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      HillClimbing(const HillClimbing&) = delete;
      HillClimbing& operator=(const HillClimbing&) = delete;

      void setMaximalStepSize(
          const arma::Col<ParameterType>& maximalStepSize);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<ParameterType> maximalStepSize_;

      void optimiseImplementation() noexcept override;

      void setDefaultMaximalStepSize(std::true_type) noexcept;
      void setDefaultMaximalStepSize(std::false_type) noexcept;
  };

  template <typename ParameterType, class DistanceFunction>
  HillClimbing<ParameterType, DistanceFunction>::HillClimbing(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : TrajectoryBasedAlgorithm<ParameterType, DistanceFunction>(optimisationProblem) {
    setDefaultMaximalStepSize(std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType, class DistanceFunction>
  void HillClimbing<ParameterType, DistanceFunction>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->optimisationProblem_->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->optimisationProblem_->getObjectiveValue(this->initialParameter_);

    while(!this->isFinished() && !this->isTerminated()) {
      ++this->numberOfIterations_;

      arma::Col<ParameterType> candidateParameter = this->distanceFunction_.getNeighbour(this->bestParameter_, maximalStepSize_);

      const arma::Col<unsigned int>& belowLowerBound = arma::find(candidateParameter < this->optimisationProblem_->getLowerBounds());
      const arma::Col<unsigned int>& aboveUpperBound = arma::find(candidateParameter > this->optimisationProblem_->getUpperBounds());

      candidateParameter.elem(belowLowerBound) = this->optimisationProblem_->getLowerBounds().elem(belowLowerBound);
      candidateParameter.elem(aboveUpperBound) = this->optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

      const double& candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
      const double& candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

      if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
        this->bestParameter_ = candidateParameter;
        this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
        this->bestObjectiveValue_ = candidateObjectiveValue;
      }
    }
  }

  template <typename ParameterType, class DistanceFunction>
  void HillClimbing<ParameterType, DistanceFunction>::setMaximalStepSize(
      const arma::Col<ParameterType>& maximalStepSize) {
    if(maximalStepSize.n_rows != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the maximal step size (" + std::to_string(maximalStepSize.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if (arma::any(maximalStepSize <= 0)) {
      throw std::logic_error("The maximal step size must be strict greater than 0.");
    }

    maximalStepSize_ = maximalStepSize;
  }

  template <typename ParameterType, class DistanceFunction>
  std::string HillClimbing<ParameterType, DistanceFunction>::to_string() const noexcept {
    return "HillClimbing";
  }

  template <typename ParameterType, class DistanceFunction>
  void HillClimbing<ParameterType, DistanceFunction>::setDefaultMaximalStepSize(
      std::true_type) noexcept {
    setMaximalStepSize(this->distanceFunction_.getDistance(this->optimisationProblem_->getLowerBounds(), this->optimisationProblem_->getUpperBounds()) / 10.0);
  }

  template <typename ParameterType, class DistanceFunction>
  void HillClimbing<ParameterType, DistanceFunction>::setDefaultMaximalStepSize(
      std::false_type) noexcept {
    setMaximalStepSize(arma::max(arma::ones<arma::Col<unsigned int>>(this->optimisationProblem_->getNumberOfDimensions()), this->distanceFunction_.getDistance(this->optimisationProblem_->getLowerBounds(), this->optimisationProblem_->getUpperBounds()) / 10.0));
  }
}
