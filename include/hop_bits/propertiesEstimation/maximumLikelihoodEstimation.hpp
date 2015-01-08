#pragma once

// HOP
#include <hop_bits/propertiesEstimation.hpp>

namespace hop {
  template <typename ParameterType, class DistanceFunction>
  class MaximumLikelihoodEstimation : public PropertiesEstimation<ParameterType, DistanceFunction> {
    public:
      using PropertiesEstimation<ParameterType, DistanceFunction>::PropertiesEstimation;

    protected:
      void estimateImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept override;
  };

  template <typename ParameterType, class DistanceFunction>
  void MaximumLikelihoodEstimation<ParameterType, DistanceFunction>::estimateImplementation(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {

    PropertiesEstimation<ParameterType, DistanceFunction>::correlationAnalysis_->analyse(optimisationProblem);
    PropertiesEstimation<ParameterType, DistanceFunction>::correlationCoefficients_.push_back(PropertiesEstimation<ParameterType, DistanceFunction>::correlationAnalysis_->getCorrelationCoefficient());

    PropertiesEstimation<ParameterType, DistanceFunction>::lipschitzContinuityAnalysis_->analyse(optimisationProblem);
    PropertiesEstimation<ParameterType, DistanceFunction>::lipschitzConstants_.push_back(PropertiesEstimation<ParameterType, DistanceFunction>::lipschitzContinuityAnalysis_->getLipschitzConstant());

    PropertiesEstimation<ParameterType, DistanceFunction>::linearModelAnalysis_->analyse(optimisationProblem);
    PropertiesEstimation<ParameterType, DistanceFunction>::linearModelEstimators_.push_back(PropertiesEstimation<ParameterType, DistanceFunction>::linearModelAnalysis_->getLinearModelEstimator());
    PropertiesEstimation<ParameterType, DistanceFunction>::isLinear_.push_back(PropertiesEstimation<ParameterType, DistanceFunction>::linearModelAnalysis_->isLinear());

    PropertiesEstimation<ParameterType, DistanceFunction>::quadraticModelAnalysis_->analyse(optimisationProblem);
    PropertiesEstimation<ParameterType, DistanceFunction>::quadraticModelEstimators_.push_back(PropertiesEstimation<ParameterType, DistanceFunction>::quadraticModelAnalysis_->getQuadraticModelEstimator());
    PropertiesEstimation<ParameterType, DistanceFunction>::isQuadratic_.push_back(PropertiesEstimation<ParameterType, DistanceFunction>::quadraticModelAnalysis_->isQuadratic());

    PropertiesEstimation<ParameterType, DistanceFunction>::additiveSeparabilityAnalysis_->analyse(optimisationProblem);

    PropertiesEstimation<ParameterType, DistanceFunction>::numberOfPropertySets_ = 1;
  }
}
