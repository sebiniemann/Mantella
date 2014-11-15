#include <hop_bits/propertiesEstimation/maximumLikelihoodEstimation.hpp>

namespace hop {
  void MaximumLikelihoodEstimation::estimateImplementation(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) {

     linearModelAnalysis_->analyse(optimisationProblem);
     linearModelEstimators_.push_back(linearModelAnalysis_->getLinearModelEstimator());
     isLinear_.push_back(false);

     quadraticModelAnalysis_->analyse(optimisationProblem);
     quadraticModelEstimators_.push_back(quadraticModelAnalysis_->getQuadraticModelEstimator());
     isQuadratic_.push_back(false);

     lipschitzContinuityAnalysis_->analyse(optimisationProblem);
     lipschitzConstants_.push_back(lipschitzContinuityAnalysis_->getLipschitzConstant());
     isLipschitzContinuous_.push_back(false);

     numberOfPropertySets_ = 1;
  }
}
