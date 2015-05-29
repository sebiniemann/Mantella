namespace mant {
  template <typename T, typename U = double>
  class OptimisationAlgorithm : public Printable {
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type.");
    static_assert(std::is_arithmetic<U>::value, "U must be an arithmetic type.");
    
    public:
      explicit OptimisationAlgorithm(
          const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept;

      void optimise();

      void setDistanceFunction(
          const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept;

      unsigned long long getNumberOfIterations() const noexcept;

      void setMaximalNumberOfIterations(
          const unsigned long long maximalNumberOfIterations) noexcept;

      arma::Col<T> getBestParameter() const noexcept;
      
      U getBestSoftConstraintsValue() const noexcept;
      
      U getBestObjectiveValue() const noexcept;
      
      bool isFinished() const noexcept;
      
      virtual bool isTerminated() const noexcept;

      std::vector<std::pair<arma::Col<T>, U>> getParameterHistory() const noexcept;
      
      virtual ~OptimisationAlgorithm() = default;

    private:
      std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem_;

    protected:
      const std::size_t numberOfDimensions_;

      unsigned long long numberOfIterations_;

      unsigned long long maximalNumberOfIterations_;

      arma::Col<T> bestParameter_;
      
      U bestSoftConstraintsValue_;
      
      U bestObjectiveValue_;

      std::vector<std::pair<arma::Col<T>, U>> parameterHistory_;

      std::shared_ptr<DistanceFunction<T>> distanceFunction_;

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

      U getAcceptableObjectiveValue() const noexcept;

      U getSoftConstraintsValue(
          const arma::Col<T>& parameter) const noexcept;

      U getObjectiveValue(
          const arma::Col<T>& parameter) noexcept;
          
      arma::Col<T> getRandomParameter() const noexcept;
          
      arma::Col<T> getRandomNeighbour(
          const arma::Col<T>& neighbour,
          const T distance) const noexcept;
          
      bool updateBestParameter(
          const arma::Col<T>& parameter,
          const U softConstraintsValue,
          const U objectiveValue) noexcept;
      
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
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

  template <typename T, typename U>
  OptimisationAlgorithm<T, U>::OptimisationAlgorithm(
      const std::shared_ptr<OptimisationProblem<T, U>> optimisationProblem) noexcept
    : optimisationProblem_(optimisationProblem),
      numberOfDimensions_(optimisationProblem_->numberOfDimensions_),
      numberOfIterations_(0),
      bestSoftConstraintsValue_(std::numeric_limits<double>::infinity()),
      bestObjectiveValue_(std::numeric_limits<double>::infinity()) {
    setMaximalNumberOfIterations(1000);

    if (std::is_integral<T>::value) {
      setDistanceFunction(std::shared_ptr<DistanceFunction<T>>(new ManhattanDistance<T>));
    }
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes_);
#else
    rank_ = 0;
    numberOfNodes_ = 1;
#endif
  }

  template <typename T, typename U>
  void OptimisationAlgorithm<T, U>::optimise() {
    verify(arma::all(optimisationProblem_->getLowerBounds() <= optimisationProblem_->getUpperBounds()), "All upper bounds of the optimisation problem must be greater than or equal to its lower bound.");
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
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
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
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

  template <typename T, typename U>
  void OptimisationAlgorithm<T, U>::setDistanceFunction(
      const std::shared_ptr<DistanceFunction<T>> distanceFunction) noexcept {
    static_assert(std::is_integral<T>::value, "T must be an integral type.");

    distanceFunction_ = distanceFunction;
  }

  template <typename T, typename U>
  unsigned long long OptimisationAlgorithm<T, U>::getNumberOfIterations() const noexcept {
    return numberOfIterations_;
  }

  template <typename T, typename U>
  void OptimisationAlgorithm<T, U>::setMaximalNumberOfIterations(
      const unsigned long long maximalNumberOfIterations) noexcept {
    maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  template <typename T, typename U>
  arma::Col<T> OptimisationAlgorithm<T, U>::getBestParameter() const noexcept {
    return bestParameter_;
  }

  template <typename T, typename U>
  U OptimisationAlgorithm<T, U>::getBestSoftConstraintsValue() const noexcept {
    return bestSoftConstraintsValue_;
  }

  template <typename T, typename U>
  U OptimisationAlgorithm<T, U>::getBestObjectiveValue() const noexcept {
    return bestObjectiveValue_;
  }

  template <typename T, typename U>
  bool OptimisationAlgorithm<T, U>::isFinished() const noexcept {
    return (isSatisfyingConstraints(bestParameter_) && bestObjectiveValue_ <= optimisationProblem_->getAcceptableObjectiveValue());
  }

  template <typename T, typename U>
  bool OptimisationAlgorithm<T, U>::isTerminated() const noexcept {
    return (numberOfIterations_ >= maximalNumberOfIterations_);
  }

  template <typename T, typename U>
  std::vector<std::pair<arma::Col<T>, U>> OptimisationAlgorithm<T, U>::getParameterHistory() const noexcept {
    return parameterHistory_;
  }

  template <typename T, typename U>
  arma::Col<T> OptimisationAlgorithm<T, U>::getLowerBounds() const noexcept {
    return optimisationProblem_->getLowerBounds();
  }

  template <typename T, typename U>
  arma::Col<T> OptimisationAlgorithm<T, U>::getUpperBounds() const noexcept {
    return optimisationProblem_->getUpperBounds();
  }

  template <typename T, typename U>
  arma::Col<unsigned int> OptimisationAlgorithm<T, U>::isWithinLowerBounds(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isWithinLowerBounds(parameter);
  }

  template <typename T, typename U>
  arma::Col<unsigned int> OptimisationAlgorithm<T, U>::isWithinUpperBounds(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isWithinUpperBounds(parameter);
  }

  template <typename T, typename U>
  bool OptimisationAlgorithm<T, U>::isSatisfyingSoftConstraints(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isSatisfyingSoftConstraints(parameter);
  }

  template <typename T, typename U>
  bool OptimisationAlgorithm<T, U>::isSatisfyingConstraints(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->isSatisfyingConstraints(parameter);
  }

  template <typename T, typename U>
  U OptimisationAlgorithm<T, U>::getAcceptableObjectiveValue() const noexcept {
    return optimisationProblem_->getAcceptableObjectiveValue();
  }

  template <typename T, typename U>
  U OptimisationAlgorithm<T, U>::getSoftConstraintsValue(
    const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
    return optimisationProblem_->getSoftConstraintsValue(parameter);
  }

  template <typename T, typename U>
  U OptimisationAlgorithm<T, U>::getObjectiveValue(
    const arma::Col<T>& parameter) noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
    
#if defined(NDEBUG)
    return optimisationProblem_->getObjectiveValue(parameter);
#else
    const U& objectiveValue = optimisationProblem_->getObjectiveValue(parameter);
    parameterHistory_.push_back({parameter, objectiveValue});
    return objectiveValue;
#endif
  }
          
  template <typename T, typename U>
  arma::Col<T> OptimisationAlgorithm<T, U>::getRandomParameter() const noexcept {
    arma::Col<T> candidateParameter;
    if (std::is_floating_point<T>::value) {
      candidateParameter = this->getLowerBounds() + arma::randu<arma::Col<T>>(this->numberOfDimensions_) % (this->getUpperBounds() - this->getLowerBounds());
    } else {
      candidateParameter.set_size(this->numberOfDimensions_);
      for (std::size_t n = 0; n < candidateParameter.n_elem; ++n) {
        candidateParameter(n) = std::uniform_int_distribution<T>(this->getLowerBounds()(n), this->getUpperBounds()(n))(Rng::getGenerator());
      }   
    }
    
    return candidateParameter;
  }
          
  template <typename T, typename U>
  arma::Col<T> OptimisationAlgorithm<T, U>::getRandomNeighbour(
      const arma::Col<T>& neighbour,
      const T distance) const noexcept {
    
  }
  
  template <typename T, typename U>
  bool OptimisationAlgorithm<T, U>::updateBestParameter(
      const arma::Col<T>& parameter,
      const U softConstraintsValue,
      const U objectiveValue) noexcept {
    assert(parameter.n_elem == numberOfDimensions_);
      
    if(softConstraintsValue < bestSoftConstraintsValue_ || (softConstraintsValue == bestSoftConstraintsValue_ && objectiveValue < this->objectiveValue)) {
      bestParameter_ = parameter;
      bestSoftConstraintsValue_ = softConstraintsValue;
      bestObjectiveValue_ = objectiveValue;
      
      return true;
    } else {
      return false;
    }
  }
  
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
  template <typename T, typename U>
  void OptimisationAlgorithm<T, U>::getBestParameter(
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
