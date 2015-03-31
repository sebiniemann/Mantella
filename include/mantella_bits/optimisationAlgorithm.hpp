namespace mant {
  template <typename ParameterType>
  class OptimisationAlgorithm : public Printable {
    public:
      // Constructs an optimisation algorithm with the given problem to be optimised.
      explicit OptimisationAlgorithm(
          // The problem to be optimised
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      // Starts the optimisation process.
      // Note: The best parameter and its objective value can be retrived via the corresponding
      // getter, after the optimisation process is finished or terminates.
      void optimise();

      void setDistanceFunction(
          const std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept;

      // Returns the current number of iterations performed.
      unsigned int getNumberOfIterations() const noexcept;

      // Sets the maximal number of iterrations to be performed.
      // The optimisation process will terminate after reaching this limit.
      void setMaximalNumberOfIterations(
          const unsigned int maximalNumberOfIterations) noexcept;

      // Returns the best parameter yet found.
      // Note: The best parameter is always selected in the following order.
      // 1 - Fulfills the interval-constraints
      // 2 - Has the lowest soft-constraint value yet found
      // 3 - Has the lowest objective value yet found
      arma::Col<ParameterType> getBestParameter() const noexcept;

      // Returns the objective value of the best parameter yet found.
      double getBestSoftConstraintsValue() const noexcept;

      // Returns the objective value of the best parameter yet found.
      double getBestObjectiveValue() const noexcept;

      std::vector<arma::Col<ParameterType>> getParameterHistory() const noexcept;

      // Checks whether the currently best objective value satisfies the upper bound for an
      // acceptable objective value.
      // Returns true if the objective value is lower than or equal to the upper bound and false
      // otherwise.
      bool isFinished() const noexcept;

      // Checks whether the current number of iterations performed violates the maximal number of
      // iterations.
      // Returns true if the number of iterations is strictly lower than the maximal number of
      // iterations
      // and false otherwise.
      virtual bool isTerminated() const noexcept;

      virtual void restart() noexcept;

      // Provides a default deconstructor.
      virtual ~OptimisationAlgorithm() = default;

    private:
      // The optimisation problem to be optimised.
      std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem_;

    protected:
      const unsigned int numberOfDimensions_;

      // The current number of iterations performed.
      unsigned int numberOfIterations_;

      // The maximal number of iterations to be performed.
      unsigned int maximalNumberOfIterations_;

      // The best parameter yet found.
      arma::Col<ParameterType> bestParameter_;

      // The best parameter's soft-constraint value.
      double bestSoftConstraintsValue_;

      // The best parameter's objective value.
      double bestObjectiveValue_;

      std::vector<arma::Col<ParameterType>> parameterHistory_;

      // The distance function to be used.
      std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction_;

      arma::Col<ParameterType> getLowerBounds() const noexcept;

      arma::Col<ParameterType> getUpperBounds() const noexcept;

      double getSoftConstraintsValue(
        const arma::Col<ParameterType>& parameter);

      arma::Col<unsigned int> isSatisfyingLowerBounds(
        const arma::Col<ParameterType>& parameter);

      arma::Col<unsigned int> isSatisfyingUpperBounds(
        const arma::Col<ParameterType>& parameter);

      bool isSatisfyingSoftConstraints(
        const arma::Col<ParameterType>& parameter);

      bool isSatisfyingConstraints(
        const arma::Col<ParameterType>& parameter);

      double getAcceptableObjectiveValue() const noexcept;

      double getObjectiveValue(
        const arma::Col<ParameterType>& parameter);

      unsigned int getNumberOfEvaluations() const noexcept;

      unsigned int getNumberOfDistinctEvaluations() const noexcept;

      void setDefaultDistanceFunction(std::true_type) noexcept;
      void setDefaultDistanceFunction(std::false_type) noexcept;

      // The actual optimisation procedere.
      // Note: The counter within the optimisation problem (counting the number of distinct function
      // evaluations for example) are already reset beforehand.
      virtual void optimiseImplementation() = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  OptimisationAlgorithm<ParameterType>::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept
    : optimisationProblem_(optimisationProblem),
      numberOfDimensions_(optimisationProblem_->numberOfDimensions_),
      numberOfIterations_(0),
      bestSoftConstraintsValue_(std::numeric_limits<double>::infinity()),
      bestObjectiveValue_(std::numeric_limits<double>::infinity()) {
    setMaximalNumberOfIterations(1000);
    setDefaultDistanceFunction(std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::optimise() {
    if(arma::any(optimisationProblem_->getUpperBounds() < optimisationProblem_->getLowerBounds())) {
      throw std::logic_error("The upper bounds of the optimisation problem must be greater than or equal to its lower bounds.");
    }
    
    // Resets the results, counters and caches
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestSoftConstraintsValue_ = std::numeric_limits<double>::infinity();
    bestParameter_.reset();
    numberOfIterations_ = 0;
    optimisationProblem_->reset();

    return optimiseImplementation();
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::setDistanceFunction(
      const std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction) noexcept {
    distanceFunction_ = distanceFunction;
  }

  template <typename ParameterType>
  unsigned int OptimisationAlgorithm<ParameterType>::getNumberOfIterations() const noexcept {
    return numberOfIterations_;
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::setMaximalNumberOfIterations(
      const unsigned int maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationAlgorithm<ParameterType>::getBestParameter() const noexcept {
    return bestParameter_;
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getBestSoftConstraintsValue() const noexcept {
    return bestSoftConstraintsValue_;
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getBestObjectiveValue() const noexcept {
    return bestObjectiveValue_;
  }

  template <typename ParameterType>
  std::vector<arma::Col<ParameterType>> OptimisationAlgorithm<ParameterType>::getParameterHistory() const noexcept {
    return parameterHistory_;
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isFinished() const noexcept {
    return (bestSoftConstraintsValue_ == 0.0 && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::restart() noexcept {

  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::setDefaultDistanceFunction(
      std::true_type) noexcept {
    setDistanceFunction(std::shared_ptr<DistanceFunction<ParameterType>>(new EuclideanDistance));
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::setDefaultDistanceFunction(
      std::false_type) noexcept {
    setDistanceFunction(std::shared_ptr<DistanceFunction<ParameterType>>(new ManhattanDistance<ParameterType>));
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationAlgorithm<ParameterType>::getLowerBounds() const noexcept {
    return optimisationProblem_->getLowerBounds();
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationAlgorithm<ParameterType>::getUpperBounds() const noexcept {
    return optimisationProblem_->getUpperBounds();
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getSoftConstraintsValue(
    const arma::Col<ParameterType>& parameter) {
    return optimisationProblem_->getSoftConstraintsValue(parameter);
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationAlgorithm<ParameterType>::isSatisfyingLowerBounds(
    const arma::Col<ParameterType>& parameter) {
    return optimisationProblem_->isSatisfyingLowerBounds(parameter);
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationAlgorithm<ParameterType>::isSatisfyingUpperBounds(
    const arma::Col<ParameterType>& parameter) {
    return optimisationProblem_->isSatisfyingUpperBounds(parameter);
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isSatisfyingSoftConstraints(
    const arma::Col<ParameterType>& parameter) {
    return optimisationProblem_->isSatisfyingSoftConstraints(parameter);
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isSatisfyingConstraints(
    const arma::Col<ParameterType>& parameter) {
    return optimisationProblem_->isSatisfyingConstraints(parameter);
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getAcceptableObjectiveValue() const noexcept {
    return optimisationProblem_->getAcceptableObjectiveValue();
  }

  template <typename ParameterType>
  double OptimisationAlgorithm<ParameterType>::getObjectiveValue(
    const arma::Col<ParameterType>& parameter) {
    parameterHistory_.push_back(parameter);
    return optimisationProblem_->getObjectiveValue(parameter);
  }

  template <typename ParameterType>
  unsigned int OptimisationAlgorithm<ParameterType>::getNumberOfEvaluations() const noexcept {
    return optimisationProblem_->getNumberOfEvaluations();
  }

  template <typename ParameterType>
  unsigned int OptimisationAlgorithm<ParameterType>::getNumberOfDistinctEvaluations() const noexcept {
    return optimisationProblem_->getNumberOfDistinctEvaluations();
  }
}
