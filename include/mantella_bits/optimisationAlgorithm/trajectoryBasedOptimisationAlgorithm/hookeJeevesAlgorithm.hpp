// TODO Add restarting
namespace mant {
  class HookeJeevesAlgorithm : public TrajectoryBasedOptimisationAlgorithm<double> {
    public:
      inline explicit HookeJeevesAlgorithm(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      inline void setInitialStepSize(
          const arma::Col<double> initialStepSize);

      inline void setStepSizeDecrease(
          const arma::Col<double> stepSizeDecrease);

      inline std::string toString() const noexcept override;

    protected:
      arma::Col<double> initialStepSize_;
      arma::Col<double> stepSizeDecrease_;

      inline void optimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  inline HookeJeevesAlgorithm::HookeJeevesAlgorithm(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept
    : TrajectoryBasedOptimisationAlgorithm<double>(optimisationProblem) {
    setInitialStepSize(this->getUpperBounds() - this->getLowerBounds());
    setStepSizeDecrease(arma::ones<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 0.5);
  }

  inline void HookeJeevesAlgorithm::optimiseImplementation() noexcept {
    ++this->numberOfIterations_;

    this->bestParameter_ = this->initialParameter_;
    this->bestSoftConstraintsValue_ = this->getSoftConstraintsValue(this->initialParameter_);
    this->bestObjectiveValue_ = this->getObjectiveValue(this->initialParameter_);

    bool reduceStepSize = false;
    arma::Col<double> stepSize = initialStepSize_;

    while(!this->isFinished() && !this->isTerminated()) {
      if (reduceStepSize) {
        stepSize %= stepSizeDecrease_;
      }

      reduceStepSize = true;

      arma::Col<double> candidateParameter = this->bestParameter_;
      for (std::size_t n = 0; n < this->numberOfDimensions_; ++n) {
        candidateParameter(n) += stepSize(n);

        if(this->getUpperBounds()(n) < candidateParameter(n)) {
          candidateParameter(n) = this->getUpperBounds()(n);
        }

        ++this->numberOfIterations_;
        double candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
        double candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

        if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
          reduceStepSize = false;

          this->bestParameter_ = candidateParameter;
          this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          this->bestObjectiveValue_ = candidateObjectiveValue;
        }

        if (this->isFinished() || this->isTerminated()) {
          break;
        }

        candidateParameter(n) = this->bestParameter_(n) - stepSize(n);

        if(this->getLowerBounds()(n) > candidateParameter(n)) {
          candidateParameter(n) = this->getLowerBounds()(n);
        }

        ++this->numberOfIterations_;
        candidateSoftConstraintsValue = this->getSoftConstraintsValue(candidateParameter);
        candidateObjectiveValue = this->getObjectiveValue(candidateParameter);

        if(candidateSoftConstraintsValue < this->bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == this->bestSoftConstraintsValue_ && candidateObjectiveValue < this->bestObjectiveValue_)) {
          reduceStepSize = false;

          this->bestParameter_ = candidateParameter;
          this->bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          this->bestObjectiveValue_ = candidateObjectiveValue;
        }

        if (this->isFinished() || this->isTerminated()) {
          break;
        }

        candidateParameter(n) = this->bestParameter_(n);
      }
    }
  }

  inline void HookeJeevesAlgorithm::setInitialStepSize(
      const arma::Col<double> initialStepSize) {
    if(initialStepSize.n_rows != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the initial step size (" + std::to_string(initialStepSize.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    } else if (arma::any(initialStepSize <= 0)) {
      throw std::logic_error("The initial step size must be strict greater than 0.");
    }

    initialStepSize_ = initialStepSize;
  }

  inline void HookeJeevesAlgorithm::setStepSizeDecrease(
      const arma::Col<double> stepSizeDecrease) {
    if(stepSizeDecrease.n_rows != this->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the step size decrease (" + std::to_string(stepSizeDecrease.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->numberOfDimensions_) + ").");
    } else if(arma::any(stepSizeDecrease <= 0) || arma::any(stepSizeDecrease >= 1)) {
      throw std::logic_error("The step size decrease must be strict greater than 0 and strict lower than 1.");
    }

    stepSizeDecrease_ = stepSizeDecrease;
  }

  inline std::string HookeJeevesAlgorithm::toString() const noexcept {
    return "HookeJeevesAlgorithm";
  }
}
