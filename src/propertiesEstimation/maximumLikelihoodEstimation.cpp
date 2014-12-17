#include <hop_bits/propertiesEstimation/maximumLikelihoodEstimation.hpp>

#include <iostream>

namespace hop {
  void MaximumLikelihoodEstimation::estimateImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {

    correlationAnalysis_->analyse(optimisationProblem);
    correlationCoefficients_.push_back(correlationAnalysis_->getCorrelationCoefficient());

    lipschitzContinuityAnalysis_->analyse(optimisationProblem);
    lipschitzConstants_.push_back(lipschitzContinuityAnalysis_->getLipschitzConstant());

    linearModelAnalysis_->analyse(optimisationProblem);
    linearModelEstimators_.push_back(linearModelAnalysis_->getLinearModelEstimator());
    isLinear_.push_back(linearModelAnalysis_->isLinear());

    quadraticModelAnalysis_->analyse(optimisationProblem);
    quadraticModelEstimators_.push_back(quadraticModelAnalysis_->getQuadraticModelEstimator());
    isQuadratic_.push_back(quadraticModelAnalysis_->isQuadratic());

    additiveSeparabilityAnalysis_->analyse(optimisationProblem);

    numberOfPropertySets_ = 1;
  }
}
