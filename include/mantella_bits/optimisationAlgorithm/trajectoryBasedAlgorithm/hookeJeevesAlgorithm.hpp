#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>
#include <mantella_bits/helper/random.hpp>

// TODO Add restarting
namespace mant {
  template <class DistanceFunction>
  class HookeJeevesAlgorithm : public TrajectoryBasedAlgorithm<double, DistanceFunction> {
    public:
      explicit HookeJeevesAlgorithm(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      HookeJeevesAlgorithm(const HookeJeevesAlgorithm&) = delete;
      HookeJeevesAlgorithm& operator=(const HookeJeevesAlgorithm&) = delete;

      void setInitialStepSize(
          const arma::Col<double>& initialStepSize);

      void setStepSizeDecrease(
          const arma::Col<double>& stepSizeDecrease);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> initialStepSize_;
      arma::Col<double> stepSizeDecrease_;

      void optimiseImplementation() noexcept override;
  };

  template <class DistanceFunction>
  HookeJeevesAlgorithm<DistanceFunction>::HookeJeevesAlgorithm(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept
    : TrajectoryBasedAlgorithm<double, DistanceFunction>(optimisationProblem) {
    setInitialStepSize(this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds());
    setStepSizeDecrease(arma::ones<arma::Col<double>>(optimisationProblem->getNumberOfDimensions()) * 0.5);
  }

  template <class DistanceFunction>
  void HookeJeevesAlgorithm<DistanceFunction>::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->optimisationProblem_->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->optimisationProblem_->getObjectiveValue(this->initialParameter_);

    bool reduceStepSize = false;
    arma::Col<double> stepSize = initialStepSize_;

    while(!this->isFinished() && !this->isTerminated()) {
      if (reduceStepSize) {
        stepSize %= stepSizeDecrease_;
      }

      reduceStepSize = true;

      arma::Col<double> candidateParameter = this->bestParameter_;
      for (std::size_t n = 0; n < this->optimisationProblem_->getNumberOfDimensions(); ++n) {
        candidateParameter.at(n) += stepSize.at(n);

        if(this->optimisationProblem_->getUpperBounds().at(n) < candidateParameter.at(n)) {
          candidateParameter.at(n) = this->optimisationProblem_->getUpperBounds().at(n);
        }

        ++this->numberOfIterations_;
        double candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
        double candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

        if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
          reduceStepSize = false;

          this->bestParameter_ = candidateParameter;
          this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          this->bestObjectiveValue_ = candidateObjectiveValue;
        }

        if (this->isFinished() || this->isTerminated()) {
          break;
        }

        candidateParameter.at(n) = this->bestParameter_.at(n) - stepSize.at(n);

        if(this->optimisationProblem_->getLowerBounds().at(n) > candidateParameter.at(n)) {
          candidateParameter.at(n) = this->optimisationProblem_->getLowerBounds().at(n);
        }

        ++this->numberOfIterations_;
        candidateSoftConstraintsValue = this->optimisationProblem_->getSoftConstraintsValue(candidateParameter);
        candidateObjectiveValue = this->optimisationProblem_->getObjectiveValue(candidateParameter);

        if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
          reduceStepSize = false;

          this->bestParameter_ = candidateParameter;
          this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          this->bestObjectiveValue_ = candidateObjectiveValue;
        }

        if (this->isFinished() || this->isTerminated()) {
          break;
        }

        candidateParameter.at(n) = this->bestParameter_.at(n);
      }
    }
  }

  template <class DistanceFunction>
  void HookeJeevesAlgorithm<DistanceFunction>::setInitialStepSize(
      const arma::Col<double>& initialStepSize) {
    if(initialStepSize.n_rows != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the initial step size (" + std::to_string(initialStepSize.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if (arma::any(initialStepSize <= 0)) {
      throw std::logic_error("The initial step size must be strict greater than 0.");
    }

    initialStepSize_ = initialStepSize;
  }

  template <class DistanceFunction>
  void HookeJeevesAlgorithm<DistanceFunction>::setStepSizeDecrease(
      const arma::Col<double>& stepSizeDecrease) {
    if(stepSizeDecrease.n_rows != this->optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the step size decrease (" + std::to_string(stepSizeDecrease.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::any(stepSizeDecrease <= 0) || arma::any(stepSizeDecrease >= 1)) {
      throw std::logic_error("The step size decrease must be strict greater than 0 and strict lower than 1.");
    }

    stepSizeDecrease_ = stepSizeDecrease;
  }

  template <class DistanceFunction>
  std::string HookeJeevesAlgorithm<DistanceFunction>::to_string() const noexcept {
    return "HookeJeevesAlgorithm";
  }
}
