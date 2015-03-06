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

      // Provides a default deconstructor.
      virtual ~OptimisationAlgorithm() = default;

    protected:
      // The optimisation problem to be optimised.
      std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem_;

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

      // The distance function to be used.
      std::shared_ptr<DistanceFunction<ParameterType>> distanceFunction_;

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
      numberOfIterations_(0),
      bestSoftConstraintsValue_(std::numeric_limits<double>::infinity()),
      bestObjectiveValue_(std::numeric_limits<double>::infinity()) {
    setMaximalNumberOfIterations(1000);
    setDefaultDistanceFunction(std::is_floating_point<ParameterType>());
  }

  template <typename ParameterType>
  void OptimisationAlgorithm<ParameterType>::optimise() {
    if(arma::any(optimisationProblem->getUpperBound() < optimisationProblem->getLowerBound())) {
      throw std::logic_error("The upper bound of the optimisation problem must be greater than or equal to its lower bound.");
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
  bool OptimisationAlgorithm<ParameterType>::isFinished() const noexcept {
    return (bestSoftConstraintsValue_ == 0.0 && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  template <typename ParameterType>
  bool OptimisationAlgorithm<ParameterType>::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
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
}
