namespace mant {
  template <typename T = double>
  class OptimisationAlgorithm : public Printable {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      explicit OptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept;

      void optimise();

      unsigned long long getNumberOfIterations() const noexcept;

      void setMaximalNumberOfIterations(
          const unsigned long long maximalNumberOfIterations) noexcept;

      arma::Col<T> getBestParameter() const noexcept;
      
      double getBestSoftConstraintsValue() const noexcept;
      
      double getBestObjectiveValue() const noexcept;
      
      bool isFinished() const noexcept;
      
      virtual bool isTerminated() const noexcept;

      std::vector<std::pair<arma::Col<T>, double>> getParameterHistory() const noexcept;
      
      virtual ~OptimisationAlgorithm() = default;

    private:
      std::shared_ptr<OptimisationProblem<T>> optimisationProblem_;

    protected:
      const std::size_t numberOfDimensions_;

      unsigned long long numberOfIterations_;

      unsigned long long maximalNumberOfIterations_;

      arma::Col<T> bestParameter_;
      
      double bestSoftConstraintsValue_;
      
      double bestObjectiveValue_;

      std::vector<std::pair<arma::Col<T>, double>> parameterHistory_;

      int rank_;
      
      int numberOfNodes_;

      arma::Col<T> getLowerBounds() const noexcept;
      
      arma::Col<T> getUpperBounds() const noexcept;

      arma::Col<unsigned int> isWithinLowerBounds(
          const arma::Col<T>& parameter) const noexcept;
        
      arma::Col<unsigned int> isWithinUpperBounds(
          const arma::Col<T>& parameter) const noexcept;
        
      bool isSatisfyingSoftConstraints(
          const arma::Col<T>& parameter) const noexcept;
        
      bool isSatisfyingConstraints(
          const arma::Col<T>& parameter) const noexcept;

      double getAcceptableObjectiveValue() const noexcept;

      double getSoftConstraintsValue(
          const arma::Col<T>& parameter) const noexcept;

      double getObjectiveValue(
          const arma::Col<T>& parameter) noexcept;
      
      arma::Col<T> getRandomParameter() const noexcept;
     
      arma::Col<T> getRandomNeighbour(
          const arma::Col<T>& parameter,
          const T minimalDistance,
          const T maximalDistance);
          
      bool updateBestParameter(
          const arma::Col<T>& parameter,
          const double softConstraintsValue,
          const double objectiveValue) noexcept;
      
#if defined(MANTELLA_USE_MPI)
      void getBestParameter(
          void* firstInput,
          void* secondInput,
          int* size,
          MPI_Datatype* type) const noexcept;
#endif

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
    
#if defined(MANTELLA_USE_MPI)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
#else
    rank_ = 0;
    numberOfNodes_ = 1;
#endif
  }

  template <typename T>
  void OptimisationAlgorithm<T>::optimise() {
    verify(arma::all(optimisationProblem_->getLowerBounds() <= optimisationProblem_->getUpperBounds()), "All upper bounds of the optimisation problem must be greater than or equal to its lower bound.");
    
#if defined(MANTELLA_USE_MPI)
    std::vector<double> serialisedOptimisationProblem;
    unsigned int serialisedOptimisationProblemSize;

    if (rank_ == 0) {
      serialisedOptimisationProblem = optimisationProblem_->serialise();
      serialisedOptimisationProblemSize = static_cast<unsigned int>(serialisedOptimisationProblem.size());
    }

    MPI_Bcast(&serialisedOptimisationProblemSize, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    if (rank_ != 0) {
      serialisedOptimisationProblem.resize(serialisedOptimisationProblemSize);
    }

    MPI_Bcast(&serialisedOptimisationProblem[0], serialisedOptimisationProblemSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank_ != 0) {
      optimisationProblem_->deserialise(serialisedOptimisationProblem);
    }
#endif
    
    // Resets the results, counters and caches
    bestObjectiveValue_ = std::numeric_limits<double>::infinity();
    bestSoftConstraintsValue_ = std::numeric_limits<double>::infinity();
    bestParameter_.reset();
    numberOfIterations_ = 0;
    optimisationProblem_->reset();

    optimiseImplementation();
    
#if defined(MANTELLA_USE_MPI)
    MPI_Datatype MANT_MPI_PARAMETER;
    MPI_Type_contiguous(2 + numberOfDimensions_, MPI_DOUBLE, &MANT_MPI_PARAMETER);
    MPI_Type_commit(&MANT_MPI_PARAMETER);
  
    MPI_Op MANT_MPI_GET_BEST_PARAMETER;
    MPI_Op_create(getBestParameter, true, &MANT_MPI_GET_BEST_PARAMETER);
    
    arma::Col<double> mpiInputParameter(2 + numberOfDimensions_);
    arma::Col<double> mpiOutputParameter(2 + numberOfDimensions_);
    
    mpiInputParameter.at(0) = softConstraintsValue;
    mpiInputParameter.at(1) = objectiveValue;
    mpiInputParameter.tail(numberOfDimensions_) = parameter;
    
    MPI_Reduce(mpiInputParameter.memptr(), mpiOutputParameter.memptr(), 1, MANT_MPI_PARAMETER, MANT_MPI_GET_BEST_PARAMETER, 0, MPI_COMM_WORLD);

    MPI_Op_free(&MANT_MPI_GET_BEST_PARAMETER);
    MPI_Type_free(&MANT_MPI_PARAMETER);
#endif
  }

  template <typename T>
  unsigned long long OptimisationAlgorithm<T>::getNumberOfIterations() const noexcept {
    return numberOfIterations_;
  }

  template <typename T>
  void OptimisationAlgorithm<T>::setMaximalNumberOfIterations(
      const unsigned long long maximalNumberOfIterations) noexcept {
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
  bool OptimisationAlgorithm<T>::isFinished() const noexcept {
    return (isSatisfyingConstraints(bestParameter_) && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  template <typename T>
  std::vector<std::pair<arma::Col<T>, double>> OptimisationAlgorithm<T>::getParameterHistory() const noexcept {
    return parameterHistory_;
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
  arma::Col<unsigned int> OptimisationAlgorithm<T>::isWithinLowerBounds(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isWithinLowerBounds(parameter);
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationAlgorithm<T>::isWithinUpperBounds(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isWithinUpperBounds(parameter);
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isSatisfyingSoftConstraints(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isSatisfyingSoftConstraints(parameter);
  }

  template <typename T>
  bool OptimisationAlgorithm<T>::isSatisfyingConstraints(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isSatisfyingConstraints(parameter);
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getAcceptableObjectiveValue() const noexcept {
    return optimisationProblem_->getAcceptableObjectiveValue();
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getSoftConstraintsValue(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->getSoftConstraintsValue(parameter);
  }

  template <typename T>
  double OptimisationAlgorithm<T>::getObjectiveValue(
    const arma::Col<T>& parameter) noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
#if defined(NDEBUG)
    return optimisationProblem_->getObjectiveValue(parameter);
#else
    const double& objectiveValue = optimisationProblem_->getObjectiveValue(parameter);
    parameterHistory_.push_back({parameter, objectiveValue});
    return objectiveValue;
#endif
  }
  
  template <typename T>
  arma::Col<T> OptimisationAlgorithm<T>::getRandomParameter() const noexcept {
    return getLowerBounds() + arma::randu<arma::Col<T>>(numberOfDimensions_) % (getUpperBounds() - getLowerBounds());
  }
  
  template <typename T>
  arma::Col<T> OptimisationAlgorithm<T>::getRandomNeighbour(
      const arma::Col<T>& parameter,
      const T minimalDistance,
      const T maximalDistance) {
    arma::Col<T> neighbour = parameter + arma::normalise(arma::randn<arma::Col<T>>(parameter.n_elem)) * std::uniform_real_distribution<T>(minimalDistance, maximalDistance)(Rng::getGenerator());
      
    const arma::Col<unsigned int>& belowLowerBound = arma::find(neighbour < getLowerBounds());
    const arma::Col<unsigned int>& aboveUpperBound = arma::find(neighbour > getUpperBounds());

    neighbour.elem(belowLowerBound) = getLowerBounds().elem(belowLowerBound);
    neighbour.elem(aboveUpperBound) = getUpperBounds().elem(aboveUpperBound);
    
    return neighbour;
  }
  
  template <typename T>
  bool OptimisationAlgorithm<T>::updateBestParameter(
      const arma::Col<T>& parameter,
      const double softConstraintsValue,
      const double objectiveValue) noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
      
    if(softConstraintsValue < bestSoftConstraintsValue_ || (softConstraintsValue == bestSoftConstraintsValue_ && objectiveValue < objectiveValue)) {
      bestParameter_ = parameter;
      bestSoftConstraintsValue_ = softConstraintsValue;
      bestObjectiveValue_ = objectiveValue;
      
      return true;
    } else {
      return false;
    }
  }
  
#if defined(MANTELLA_USE_MPI)
  template <typename T>
  void OptimisationAlgorithm<T>::getBestParameter(
      void* firstInput,
      void* secondInput,
      int* size,
      MPI_Datatype* type) const noexcept {
    double* firstParameters = static_cast<double*>(firstInput);
    double* secondParameters = static_cast<double*>(secondInput);
    
    if(firstParameters[0] < secondParameters[0] || (firstParameters[0] == secondParameters[0] && firstParameters[0 + 1] < secondParameters[0 + 1])) {
      std::copy(&firstParameters[0], &firstParameters[0 + numberOfDimensions_ - 1], &secondParameters[0]);
    }
  }
#endif
}
