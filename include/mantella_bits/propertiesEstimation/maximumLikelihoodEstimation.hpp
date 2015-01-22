namespace mant {
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

    this->correlationAnalysis_->analyse(optimisationProblem);
    this->correlationCoefficients_.push_back(this->correlationAnalysis_->getCorrelationCoefficient());

    this->lipschitzContinuityAnalysis_->analyse(optimisationProblem);
    this->lipschitzConstants_.push_back(this->lipschitzContinuityAnalysis_->getLipschitzConstant());

    this->linearModelAnalysis_->analyse(optimisationProblem);
    this->linearModelEstimators_.push_back(this->linearModelAnalysis_->getLinearModelEstimator());
    this->isLinear_.push_back(this->linearModelAnalysis_->isLinear());

    this->quadraticModelAnalysis_->analyse(optimisationProblem);
    this->quadraticModelEstimators_.push_back(this->quadraticModelAnalysis_->getQuadraticModelEstimator());
    this->isQuadratic_.push_back(this->quadraticModelAnalysis_->isQuadratic());

    this->additiveSeparabilityAnalysis_->analyse(optimisationProblem);

    this->numberOfPropertySets_ = 1;
  }
}
