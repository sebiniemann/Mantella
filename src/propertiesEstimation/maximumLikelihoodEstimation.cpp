#include <hop_bits/propertiesEstimation/maximumLikelihoodEstimation.hpp>

#include <iostream>

namespace hop {
  void MaximumLikelihoodEstimation::estimateImplementation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept {

    correlationAnalysis_->analyse(optimisationProblem);
    correlationCoefficient_.push_back(correlationAnalysis_->getCorrelationCoefficient());

    lipschitzContinuityAnalysis_->analyse(optimisationProblem);
    lipschitzConstants_.push_back(lipschitzContinuityAnalysis_->getLipschitzConstant());

    linearModelAnalysis_->analyse(optimisationProblem);
    linearModelEstimators_.push_back(linearModelAnalysis_->getLinearModelEstimator());
    isLinear_.push_back(false);

    quadraticModelAnalysis_->analyse(optimisationProblem);
    quadraticModelEstimators_.push_back(quadraticModelAnalysis_->getQuadraticModelEstimator());
    isQuadratic_.push_back(false);

    additiveSeparabilityAnalysis_->analyse(optimisationProblem);

    numberOfPropertySets_ = 1;
  }
}
