#include <mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm/hookeJeevesAlgorithm.hpp>

// Mantella
#include <mantella_bits/helper/random.hpp>

// TODO Add restarting
namespace mant {
  HookeJeevesAlgorithm::HookeJeevesAlgorithm(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept
    : TrajectoryBasedAlgorithm<double, EuclideanDistance>(optimisationProblem) {
    setInitialStepSize(optimisationProblem_->getUpperBounds() - optimisationProblem_->getLowerBounds());
    setStepSizeDecrease(arma::ones<arma::Col<double>>(optimisationProblem->getNumberOfDimensions()) * 0.5);
  }

  void HookeJeevesAlgorithm::optimiseImplementation() noexcept {
    ++numberOfIterations_;

    bestParameter_ = initialParameter_;
    bestSoftConstraintsValue_ = optimisationProblem_->getSoftConstraintsValue(initialParameter_);
    bestObjectiveValue_ = optimisationProblem_->getObjectiveValue(initialParameter_);

    bool reduceStepSize = false;
    arma::Col<double> stepSize = initialStepSize_;
    
    while(!isFinished() && !isTerminated()) {
      if (reduceStepSize) {
        stepSize %= stepSizeDecrease_;
      }

      reduceStepSize = true;

      arma::Col<double> candidateParameter = bestParameter_;
      for (std::size_t n = 0; n < optimisationProblem_->getNumberOfDimensions(); ++n) {
        candidateParameter.at(n) += stepSize.at(n);
        
        if(optimisationProblem_->getUpperBounds().at(n) < candidateParameter.at(n)) {
          candidateParameter.at(n) = optimisationProblem_->getUpperBounds().at(n);
        }

        ++numberOfIterations_;
        double candidateSoftConstraintsValue = optimisationProblem_->getSoftConstraintsValue(candidateParameter);
        double candidateObjectiveValue = optimisationProblem_->getObjectiveValue(candidateParameter);

        if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_)) {
          reduceStepSize = false;

          bestParameter_ = candidateParameter;
          bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          bestObjectiveValue_ = candidateObjectiveValue;
        }

        if (isFinished() || isTerminated()) {
          break;
        }

        candidateParameter.at(n) = bestParameter_.at(n) - stepSize.at(n);

        if(optimisationProblem_->getLowerBounds().at(n) > candidateParameter.at(n)) {
          candidateParameter.at(n) = optimisationProblem_->getLowerBounds().at(n);
        }

        ++numberOfIterations_;
        candidateSoftConstraintsValue = optimisationProblem_->getSoftConstraintsValue(candidateParameter);
        candidateObjectiveValue = optimisationProblem_->getObjectiveValue(candidateParameter);

        if(candidateSoftConstraintsValue < bestSoftConstraintsValue_ || (candidateSoftConstraintsValue == bestSoftConstraintsValue_ && candidateObjectiveValue < bestObjectiveValue_)) {
          reduceStepSize = false;

          bestParameter_ = candidateParameter;
          bestSoftConstraintsValue_ = candidateSoftConstraintsValue;
          bestObjectiveValue_ = candidateObjectiveValue;
        }

        if (isFinished() || isTerminated()) {
          break;
        }

        candidateParameter.at(n) = bestParameter_.at(n);
      }
    }
  }

  void HookeJeevesAlgorithm::setInitialStepSize(
      const arma::Col<double>& initialStepSize) {
    if(initialStepSize.n_rows != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the initial step size (" + std::to_string(initialStepSize.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if (arma::any(initialStepSize <= 0)) {
      throw std::logic_error("The initial step size must be strict greater than 0.");
    }

    initialStepSize_ = initialStepSize;
  }

  void HookeJeevesAlgorithm::setStepSizeDecrease(
      const arma::Col<double>& stepSizeDecrease) {
    if(stepSizeDecrease.n_rows != optimisationProblem_->getNumberOfDimensions()) {
      throw std::logic_error("The number of dimensions of the step size decrease (" + std::to_string(stepSizeDecrease.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(optimisationProblem_->getNumberOfDimensions()) + ").");
    } else if(arma::any(stepSizeDecrease <= 0) || arma::any(stepSizeDecrease >= 1)) {
      throw std::logic_error("The step size decrease must be strict greater than 0 and strict lower than 1.");
    }

    stepSizeDecrease_ = stepSizeDecrease;
  }

  std::string HookeJeevesAlgorithm::to_string() const noexcept {
    return "HookeJeevesAlgorithm";
  }
}
