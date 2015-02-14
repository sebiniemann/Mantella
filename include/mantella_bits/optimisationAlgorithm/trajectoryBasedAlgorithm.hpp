namespace mant {
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

  //
  // Implementation
  //

  template <typename ParameterType>
  TrajectoryBasedAlgorithm<ParameterType>::TrajectoryBasedAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : OptimisationAlgorithm<ParameterType>(optimisationProblem) {
    // TODO fix for unsigned int
    setInitialParameter(arma::randu<arma::Col<ParameterType>>(this->optimisationProblem_->numberOfDimensions_) % (this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds()) + this->optimisationProblem_->getLowerBounds());
  }

  template <typename ParameterType>
  void TrajectoryBasedAlgorithm<ParameterType>::setInitialParameter(
      const arma::Col<ParameterType>& initialParameter) {
    if(initialParameter.n_elem != this->optimisationProblem_->numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the initial parameter (" + std::to_string(initialParameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(this->optimisationProblem_->numberOfDimensions_) + ").");
    }

    initialParameter_ = initialParameter;
  }
}
