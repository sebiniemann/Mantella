namespace mant {
  template <typename T = double>
  class OptimisationProblem : public Printable {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      const std::size_t numberOfDimensions_;

      explicit OptimisationProblem(
        const std::size_t numberOfDimensions) noexcept;

      void setLowerBounds(
        const arma::Col<T>& lowerBounds);

      void setUpperBounds(
        const arma::Col<T>& upperBounds);

      arma::Col<T> getLowerBounds() const noexcept;

      arma::Col<T> getUpperBounds() const noexcept;

      double getSoftConstraintsValue(
        const arma::Col<T>& parameter);

      arma::Col<unsigned int> isWithinLowerBounds(
        const arma::Col<T>& parameter);

      arma::Col<unsigned int> isWithinUpperBounds(
        const arma::Col<T>& parameter);

      bool isSatisfyingSoftConstraints(
        const arma::Col<T>& parameter);

      bool isSatisfyingConstraints(
        const arma::Col<T>& parameter);

      void setParameterPermutation(
          const arma::Col<unsigned int>& parameterPermutation);

      void setParameterScaling(
        const arma::Col<T>& parameterScaling);

      void setParameterTranslation(
          const arma::Col<T>& parameterTranslation);

      void setParameterRotation(
        const arma::Mat<T>& parameterRotation);

      void setObjectiveValueScaling(
        const double objectiveValueScaling);

      void setObjectiveValueTranslation(
        const double objectiveValueTranslation);
      
      void setAcceptableObjectiveValue(
          const double acceptableObjectiveValue) noexcept;
        
      double getAcceptableObjectiveValue() const noexcept;

      double getObjectiveValue(
        const arma::Col<T>& parameter);

      unsigned long long getNumberOfEvaluations() const noexcept;

      unsigned long long getNumberOfDistinctEvaluations() const noexcept;
      
      void reset() noexcept;

      std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>> getCachedObjectiveValues() const noexcept;

      virtual ~OptimisationProblem() = default;

    protected:
      arma::Col<T> lowerBounds_;
      arma::Col<T> upperBounds_;

      arma::Col<unsigned int> parameterPermutation_;
      arma::Col<T> parameterScaling_;
      arma::Col<T> parameterTranslation_;
      arma::Mat<T> parameterRotation_;

      double objectiveValueScaling_;
      double objectiveValueTranslation_;

      double acceptableObjectiveValue_;

      unsigned long long numberOfEvaluations_;
      unsigned long long numberOfDistinctEvaluations_;

      arma::Col<T> getDiversifiedParameter(
        const arma::Col<T>& parameter) const noexcept;

      virtual double getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept;

      virtual double getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept = 0;

      std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>> cachedObjectiveValues_;

#if defined(MANTELLA_USE_MPI)
      // Grants direct access to the otherwise hidden .serialise() and .deserialise(...) methods.
      friend class OptimisationAlgorithm;

      // The type is intentionally fixed to ease usage with MPI_DOUBLE.
      std::vector<double> serialise() const noexcept;

      // The type is intentionally fixed to ease usage with MPI_DOUBLE.
      void deserialise(
          const std::vector<double>& serialisedOptimisationProblem);
#endif
  };

  //
  // Implementation
  //

  template <typename T>
  OptimisationProblem<T>::OptimisationProblem(
      const std::size_t numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions) {
    reset();
    
    // A vector with all elements set to the lowest representable value.
    setLowerBounds(arma::zeros<arma::Col<T>>(numberOfDimensions_) - std::numeric_limits<T>::max());
    // A vector with all elements set to the largest representable value.
    setUpperBounds(arma::zeros<arma::Col<T>>(numberOfDimensions_) + std::numeric_limits<T>::max());
    
    // (0, ..., numberOfDimensions - 1) 
    setParameterPermutation(arma::linspace<arma::Col<unsigned int>>(0, numberOfDimensions_ - 1, numberOfDimensions));
      
    setParameterScaling(arma::ones<arma::Col<T>>(numberOfDimensions_));
    setParameterTranslation(arma::zeros<arma::Col<T>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<T>>(numberOfDimensions_, numberOfDimensions_));
    
    setObjectiveValueScaling(1.0);
    setObjectiveValueTranslation(0.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  template <typename T>
  void OptimisationProblem<T>::setLowerBounds(
      const arma::Col<T>& lowerBounds) {
    verify(lowerBounds.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    lowerBounds_ = lowerBounds;
  }

  template <typename T>
  void OptimisationProblem<T>::setUpperBounds(
      const arma::Col<T>& upperBounds) {
    verify(upperBounds.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    upperBounds_ = upperBounds;
  }

  template <typename T>
  arma::Col<T> OptimisationProblem<T>::getLowerBounds() const noexcept {
    return lowerBounds_;
  }

  template <typename T>
  arma::Col<T> OptimisationProblem<T>::getUpperBounds() const noexcept {
    return upperBounds_;
  }

  template <typename T>
  double OptimisationProblem<T>::getSoftConstraintsValue(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    const double softConstraintValue = getSoftConstraintsValueImplementation(parameter);
    
    // All soft-constraint values must be greater than or equal to 0.
    assert(softConstraintValue >= 0.0);
    
    return objectiveValueScaling_ * softConstraintValue;
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationProblem<T>::isWithinLowerBounds(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return parameter >= lowerBounds_;
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationProblem<T>::isWithinUpperBounds(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return  parameter <= upperBounds_;
  }

  template <typename T>
  bool OptimisationProblem<T>::isSatisfyingSoftConstraints(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return (getSoftConstraintsValue(parameter) == 0.0);
  }

  template <typename T>
  bool OptimisationProblem<T>::isSatisfyingConstraints(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return (arma::all(isWithinLowerBounds(parameter)) && arma::all(isWithinUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));
  }

  template <typename T>
  void OptimisationProblem<T>::setParameterPermutation(
      const arma::Col<unsigned int>& parameterPermutation) {
    verify(parameterPermutation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions");
    verify(isPermutation(parameterPermutation, numberOfDimensions_, numberOfDimensions_), "The parameter must be a permutation.");

    parameterPermutation_ = parameterPermutation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::setParameterScaling(
      const arma::Col<T>& parameterScaling) {
    verify(parameterScaling.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");
    verify(parameterScaling.is_finite(), "All elements must be finite.");

    parameterScaling_ = parameterScaling;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::setParameterTranslation(
      const arma::Col<T>& parameterTranslation) {
    verify(parameterTranslation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");
    verify(parameterTranslation.is_finite(), "All elements must be finite.");

    parameterTranslation_ = parameterTranslation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::setParameterRotation(
      const arma::Mat<T>& parameterRotation) {
    verify(parameterRotation.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions.");
    verify(isRotationMatrix(parameterRotation), "The parameter must be a rotation matrix.");

    parameterRotation_ = parameterRotation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) {
    verify(std::isfinite(objectiveValueScaling), "The objective value scaling must be finite.");
    
    objectiveValueScaling_ = objectiveValueScaling;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::setObjectiveValueTranslation(
      const double objectiveValueTranslation) {
    verify(std::isfinite(objectiveValueTranslation), "The objective value translation must be finite.");
    
    objectiveValueTranslation_ = objectiveValueTranslation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::setAcceptableObjectiveValue(
      const double acceptableObjectiveValue) noexcept {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  template <typename T>
  double OptimisationProblem<T>::getAcceptableObjectiveValue() const noexcept {
    return acceptableObjectiveValue_;
  }

  template <typename T>
  double OptimisationProblem<T>::getObjectiveValue(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

#if defined(MANTELLA_USE_CACHES)
    // Check if the result is already cached.
    const auto n = cachedObjectiveValues_.find(parameter);
    if (n == cachedObjectiveValues_.cend()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      const double result = objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
      
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      return n->second;
    }
#else
    // Without caching, all function evaluations must be computed.
    ++numberOfDistinctEvaluations_;
      
    return objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
#endif
  }

  template <typename T>
  unsigned long long OptimisationProblem<T>::getNumberOfEvaluations() const noexcept {
    return numberOfEvaluations_;
  }

  template <typename T>
  unsigned long long OptimisationProblem<T>::getNumberOfDistinctEvaluations() const noexcept {
    return numberOfDistinctEvaluations_;
  }

  template <typename T>
  void OptimisationProblem<T>::reset() noexcept {
    numberOfEvaluations_ = 0llu;
    numberOfDistinctEvaluations_ = 0llu;
    
    cachedObjectiveValues_.clear();
  }

  template <typename T>
  std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>> OptimisationProblem<T>::getCachedObjectiveValues() const noexcept {
    return cachedObjectiveValues_;
  }

  template <typename T>
  arma::Col<T> OptimisationProblem<T>::getDiversifiedParameter(
      const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    return parameterRotation_ * (parameterScaling_ % parameter.elem(parameterPermutation_) - parameterTranslation_);
  }

  template <typename T>
  double OptimisationProblem<T>::getSoftConstraintsValueImplementation(
      const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    return 0.0;
  }

#if defined(MANTELLA_USE_MPI)
  template <typename T>
  std::vector<double> OptimisationProblem<T>::serialise() const noexcept {
    std::vector<double> serialisedOptimisationProblem;

    for(std::size_t n = 0; n < lowerBounds_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(lowerBounds_(n)));
    }

    for(std::size_t n = 0; n < upperBounds_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(upperBounds_(n)));
    }

    for(std::size_t n = 0; n < parameterPermutation_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(parameterPermutation_(n)));
    }

    for(std::size_t n = 0; n < parameterScaling_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(parameterScaling_(n)));
    }

    for(std::size_t n = 0; n < parameterTranslation_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(parameterTranslation_(n)));
    }

    for(std::size_t n = 0; n < parameterRotation_.n_elem; ++n) {
      serialisedOptimisationProblem.push_back(static_cast<double>(parameterRotation_(n)));
    }

    serialisedOptimisationProblem.push_back(objectiveValueScaling_);
    serialisedOptimisationProblem.push_back(objectiveValueTranslation_);

    serialisedOptimisationProblem.push_back(acceptableObjectiveValue_);

    return serialisedOptimisationProblem;
  }

  template <typename T>
  void OptimisationProblem<T>::deserialise(
      const std::vector<double>& serialisedOptimisationProblem) {
    lowerBounds_.set_size(this->numberOfDimensions_);
    for(std::size_t n = 0; n < lowerBounds_.n_elem; ++n) {
      lowerBounds_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
    }
    
    upperBounds_.set_size(this->numberOfDimensions_);
    for(std::size_t n = 0; n < upperBounds_.n_elem; ++n) {
      upperBounds_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
    }
    
    parameterPermutation_.set_size(this->numberOfDimensions_);
    for(std::size_t n = 0; n < parameterPermutation_.n_elem; ++n) {
      parameterPermutation_(n) = static_cast<unsigned int>(serialisedOptimisationProblem.pop_back());
    }
    
    parameterScaling_.set_size(this->numberOfDimensions_);
    for(std::size_t n = 0; n < parameterScaling_.n_elem; ++n) {
      parameterScaling_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
    }
    
    parameterTranslation_.set_size(this->numberOfDimensions_);
    for(std::size_t n = 0; n < parameterTranslation_.n_elem; ++n) {
      parameterTranslation_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
    }
    
    parameterRotation_.set_size(this->numberOfDimensions_);
    for(std::size_t n = 0; n < parameterRotation_.n_elem; ++n) {
      parameterRotation_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
    }

    objectiveValueScaling_ = serialisedOptimisationProblem.pop_back();
    objectiveValueTranslation_ = serialisedOptimisationProblem.pop_back();

    acceptableObjectiveValue_ = serialisedOptimisationProblem.pop_back();
  }
#endif
}
