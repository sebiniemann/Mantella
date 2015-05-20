namespace mant {
  template <typename T>
  class OptimisationAlgorithm : public Printable {
    public:
      // Constructs an optimisation algorithm with the given problem to be optimised.
      explicit OptimisationAlgorithm(
          // The problem to be optimised
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      // Starts the optimisation process.
      // Note: The best parameter and its objective value can be retrived via the corresponding
      // getter, after the optimisation process is finished or terminates.
      void optimise();

      void setDistanceFunction(
          const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept;

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
      arma::Col<T> getBestParameter() const noexcept;

      // Returns the objective value of the best parameter yet found.
      double getBestSoftConstraintsValue() const noexcept;

      // Returns the objective value of the best parameter yet found.
      double getBestObjectiveValue() const noexcept;

      std::vector<arma::Col<T>> getParameterHistory() const noexcept;

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
      std::shared_ptr<OptimisationProblem<T>> optimisationProblem_;

    protected:
      const unsigned int numberOfDimensions_;

      // The current number of iterations performed.
      unsigned int numberOfIterations_;

      // The maximal number of iterations to be performed.
      unsigned int maximalNumberOfIterations_;

      // The best parameter yet found.
      arma::Col<T> bestParameter_;

      // The best parameter's soft-constraint value.
      double bestSoftConstraintsValue_;

      // The best parameter's objective value.
      double bestObjectiveValue_;

      std::vector<arma::Col<T>> parameterHistory_;

      // The distance function to be used.
      std::shared_ptr<DistanceFunction<T>> distanceFunction_;

      arma::Col<T> getLowerBounds() const noexcept;

      arma::Col<T> getUpperBounds() const noexcept;

      double getSoftConstraintsValue(
        const arma::Col<T>& parameter);

      arma::Col<unsigned int> isSatisfyingLowerBounds(
        const arma::Col<T>& parameter);

      arma::Col<unsigned int> isSatisfyingUpperBounds(
        const arma::Col<T>& parameter);

      bool isSatisfyingSoftConstraints(
        const arma::Col<T>& parameter);

      bool isSatisfyingConstraints(
        const arma::Col<T>& parameter);

      double getAcceptableObjectiveValue() const noexcept;

      double getObjectiveValue(
        const arma::Col<T>& parameter);

      unsigned int getNumberOfEvaluations() const noexcept;

      unsigned int getNumberOfDistinctEvaluations() const noexcept;

      // The actual optimisation procedere.
      // Note: The counter within the optimisation problem (counting the number of distinct function
      // evaluations for example) are already reset beforehand.
      virtual void optimiseImplementation() = 0;
  };

  //
  // Implementation
  //

  template <typename T>
  OptimisationAlgorithm<T>::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept
    : optimisationProblem_(optimisationProblem),
      numberOfDimensions_(optimisationProblem_->numberOfDimensions_),
      numberOfIterations_(0),
      bestSoftConstraintsValue_(std::numeric_limits<double>::infinity()),
      bestObjectiveValue_(std::numeric_limits<double>::infinity()) {
    setMaximalNumberOfIterations(1000);

    if(std::is_integral<T>::value) {
      setDistanceFunction(std::shared_ptr<DistanceFunction<T>>(new ManhattanDistance<T>));
    }
  }

  template <typename T>
  void OptimisationAlgorithm<T>::optimise() {
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

  template <typename T>
  void OptimisationAlgorithm<T>::setDistanceFunction(
      const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept {
    static_assert(std::is_integral<T>::value, "T must be an integral type.");

    distanceFunction_ = distanceFunction;
  }

  template <typename T>
  unsigned int OptimisationAlgorithm<T>::getNumberOfIterations() const noexcept {
    return numberOfIterations_;
  }

  template <typename T>
  void OptimisationAlgorithm<T>::setMaximalNumberOfIterations(
      const unsigned int maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  template <typename T>
  arma::Col<T> OptimisationAlgorithm<T>::getBestParameter() const noexcept {
    return bestParameter_;
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getBestSoftConstraintsValue() const noexcept {
    return bestSoftConstraintsValue_;
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getBestObjectiveValue() const noexcept {
    return bestObjectiveValue_;
  }

  template <typename T>
  std::vector<arma::Col<T>> OptimisationAlgorithm<T>::getParameterHistory() const noexcept {
    return parameterHistory_;
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isFinished() const noexcept {
    return (bestSoftConstraintsValue_ == 0.0 && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  template <typename T>
  void OptimisationAlgorithm<T>::restart() noexcept {

  }

  template <typename T>
  arma::Col<T> OptimisationAlgorithm<T>::getLowerBounds() const noexcept {
    return optimisationProblem_->getLowerBounds();
  }

  template <typename T>
  arma::Col<T> OptimisationAlgorithm<T>::getUpperBounds() const noexcept {
    return optimisationProblem_->getUpperBounds();
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getSoftConstraintsValue(
    const arma::Col<T>& parameter) {
    return optimisationProblem_->getSoftConstraintsValue(parameter);
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationAlgorithm<T>::isSatisfyingLowerBounds(
    const arma::Col<T>& parameter) {
    return optimisationProblem_->isSatisfyingLowerBounds(parameter);
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationAlgorithm<T>::isSatisfyingUpperBounds(
    const arma::Col<T>& parameter) {
    return optimisationProblem_->isSatisfyingUpperBounds(parameter);
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isSatisfyingSoftConstraints(
    const arma::Col<T>& parameter) {
    return optimisationProblem_->isSatisfyingSoftConstraints(parameter);
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isSatisfyingConstraints(
    const arma::Col<T>& parameter) {
    return optimisationProblem_->isSatisfyingConstraints(parameter);
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getAcceptableObjectiveValue() const noexcept {
    return optimisationProblem_->getAcceptableObjectiveValue();
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getObjectiveValue(
    const arma::Col<T>& parameter) {
    parameterHistory_.push_back(parameter);
    return optimisationProblem_->getObjectiveValue(parameter);
  }

  template <typename T>
  unsigned int OptimisationAlgorithm<T>::getNumberOfEvaluations() const noexcept {
    return optimisationProblem_->getNumberOfEvaluations();
  }

  template <typename T>
  unsigned int OptimisationAlgorithm<T>::getNumberOfDistinctEvaluations() const noexcept {
    return optimisationProblem_->getNumberOfDistinctEvaluations();
  }
}
