namespace mant {
  template <typename T, typename U = double>
  class OptimisationProblem : public Printable {
    static_assert(arma::is_supported_elem_type<T>::value, "The parameter type T must be supported by Armadillo C++, i.e. an arithmetic types, except bool.");
    static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
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

      U getSoftConstraintsValue(
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
        const U objectiveValueScaling) noexcept;

      void setObjectiveValueTranslation(
        const U objectiveValueTranslation) noexcept;
      
      void setAcceptableObjectiveValue(
          const U acceptableObjectiveValue) noexcept;
        
      U getAcceptableObjectiveValue() const noexcept;

      U getObjectiveValue(
        const arma::Col<T>& parameter);

      unsigned long long getNumberOfEvaluations() const noexcept;

      unsigned long long getNumberOfDistinctEvaluations() const noexcept;
      
      void reset() noexcept;

      std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>> getCachedObjectiveValues() const noexcept;

      virtual ~OptimisationProblem() = default;

    protected:
      arma::Col<T> lowerBounds_;
      arma::Col<T> upperBounds_;

      arma::Col<unsigned int> parameterPermutation_;
      arma::Col<T> parameterScaling_;
      arma::Col<T> parameterTranslation_;
      arma::Mat<T> parameterRotation_;

      U objectiveValueScaling_;
      U objectiveValueTranslation_;

      U acceptableObjectiveValue_;

      unsigned long long numberOfEvaluations_;
      unsigned long long numberOfDistinctEvaluations_;

      arma::Col<T> getDiversifiedParameter(
        const arma::Col<T>& parameter) const noexcept;

      virtual U getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept;

      virtual U getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept = 0;

      std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>> cachedObjectiveValues_;

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

  template <typename T, typename U>
  OptimisationProblem<T, U>::OptimisationProblem(
      const std::size_t numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions) {
    reset();
    
    // A vector with all elements set to the lowest representable value.
    setLowerBounds(arma::zeros<arma::Col<T>>(numberOfDimensions_) - std::numeric_limits<T>::max());
    // A vector with all elements set to the largest representable value.
    setUpperBounds(arma::zeros<arma::Col<T>>(numberOfDimensions_) + std::numeric_limits<T>::max());
    
    // (0, ..., numberOfDimensions - 1) 
    setParameterPermutation(arma::linspace<arma::Col<unsigned int>>(0, numberOfDimensions_ - 1, numberOfDimensions));
      
    if (std::is_floating_point<T>::value) {
      setParameterScaling(arma::ones<arma::Col<T>>(numberOfDimensions_));
      setParameterTranslation(arma::zeros<arma::Col<T>>(numberOfDimensions_));
      setParameterRotation(arma::eye<arma::Mat<T>>(numberOfDimensions_, numberOfDimensions_));
    }
    
    setObjectiveValueScaling(static_cast<U>(1.0L));
    setObjectiveValueTranslation(static_cast<U>(0.0L));
    setAcceptableObjectiveValue(std::numeric_limits<U>::lowest());
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setLowerBounds(
      const arma::Col<T>& lowerBounds) {
    verify(lowerBounds.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    lowerBounds_ = lowerBounds;
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setUpperBounds(
      const arma::Col<T>& upperBounds) {
    verify(upperBounds.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    upperBounds_ = upperBounds;
  }

  template <typename T, typename U>
  arma::Col<T> OptimisationProblem<T, U>::getLowerBounds() const noexcept {
    return lowerBounds_;
  }

  template <typename T, typename U>
  arma::Col<T> OptimisationProblem<T, U>::getUpperBounds() const noexcept {
    return upperBounds_;
  }

  template <typename T, typename U>
  U OptimisationProblem<T, U>::getSoftConstraintsValue(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    const U softConstraintValue = getSoftConstraintsValueImplementation(parameter);
    
    // All soft-constraint values must be greater than or equal to 0.
    assert(softConstraintValue >= static_cast<U>(0.0L));
    
    return objectiveValueScaling_ * softConstraintValue;
  }

  template <typename T, typename U>
  arma::Col<unsigned int> OptimisationProblem<T, U>::isWithinLowerBounds(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return parameter >= lowerBounds_;
  }

  template <typename T, typename U>
  arma::Col<unsigned int> OptimisationProblem<T, U>::isWithinUpperBounds(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return  parameter <= upperBounds_;
  }

  template <typename T, typename U>
  bool OptimisationProblem<T, U>::isSatisfyingSoftConstraints(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return (getSoftConstraintsValue(parameter) == static_cast<U>(0.0L));
  }

  template <typename T, typename U>
  bool OptimisationProblem<T, U>::isSatisfyingConstraints(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return (arma::all(isWithinLowerBounds(parameter)) && arma::all(isWithinUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setParameterPermutation(
      const arma::Col<unsigned int>& parameterPermutation) {
    verify(parameterPermutation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions");
    verify(isPermutation(parameterPermutation, 0, numberOfDimensions_ - 1), "The parameter must be a permutation.");

    parameterPermutation_ = parameterPermutation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setParameterScaling(
      const arma::Col<T>& parameterScaling) {
    verify(parameterScaling.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    parameterScaling_ = parameterScaling;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setParameterTranslation(
      const arma::Col<T>& parameterTranslation) {
    verify(parameterTranslation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    parameterTranslation_ = parameterTranslation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setParameterRotation(
      const arma::Mat<T>& parameterRotation) {
    verify(parameterRotation.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions.");
    verify(isRotationMatrix(parameterRotation), "The parameter must be a rotation matrix.");

    parameterRotation_ = parameterRotation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::OptimisationProblem::setObjectiveValueScaling(
      const U objectiveValueScaling) noexcept {
    objectiveValueScaling_ = objectiveValueScaling;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setObjectiveValueTranslation(
      const U objectiveValueTranslation) noexcept {
    objectiveValueTranslation_ = objectiveValueTranslation;

    // Resets all counters and caches, as the problem could be changed.
    reset();
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::setAcceptableObjectiveValue(
      const U acceptableObjectiveValue) noexcept {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  template <typename T, typename U>
  U OptimisationProblem<T, U>::getAcceptableObjectiveValue() const noexcept {
    return acceptableObjectiveValue_;
  }

  template <typename T, typename U>
  U OptimisationProblem<T, U>::getObjectiveValue(
      const arma::Col<T>& parameter)
  {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

#if defined(MANTELLA_USE_CACHES)
    // Check if the result is already cached.
    const auto& index = cachedObjectiveValues_.find(parameter);
    if (index == cachedObjectiveValues_.cend()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      const U result = objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
      
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      return index->second;
    }
#else
    // Without caching, all function evaluations must be computed.
    ++numberOfDistinctEvaluations_;
      
    return objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
#endif
  }

  template <typename T, typename U>
  unsigned long long OptimisationProblem<T, U>::getNumberOfEvaluations() const noexcept {
    return numberOfEvaluations_;
  }

  template <typename T, typename U>
  unsigned long long OptimisationProblem<T, U>::getNumberOfDistinctEvaluations() const noexcept {
    return numberOfDistinctEvaluations_;
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::reset() noexcept {
    numberOfEvaluations_ = 0llu;
    numberOfDistinctEvaluations_ = 0llu;
    
    cachedObjectiveValues_.clear();
  }

  template <typename T, typename U>
  std::unordered_map<arma::Col<T>, U, Hash<T>, IsEqual<T>> OptimisationProblem<T, U>::getCachedObjectiveValues() const noexcept {
    return cachedObjectiveValues_;
  }

  template <typename T, typename U>
  arma::Col<T> OptimisationProblem<T, U>::getDiversifiedParameter(
      const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    if (std::is_floating_point<T>::value) {
      // For continuous problems, parameter is firstly permuted, than scaled, translated and lastly rotated.
      return parameterRotation_ * (parameterScaling_ % parameter.elem(parameterPermutation_) - parameterTranslation_);
    } else {
      // For discrete problems, the parameter is only permuted.
      return parameter.elem(parameterPermutation_);
    }
  }

  template <typename T, typename U>
  U OptimisationProblem<T, U>::getSoftConstraintsValueImplementation(
      const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    return static_cast<U>(0.0L);
  }

#if defined(MANTELLA_USE_MPI)
  template <typename T, typename U>
  std::vector<double> OptimisationProblem<T, U>::serialise() const noexcept {
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

    if (std::is_floating_point<T>::value) {
      for(std::size_t n = 0; n < parameterScaling_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(parameterScaling_(n)));
      }

      for(std::size_t n = 0; n < parameterTranslation_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(parameterTranslation_(n)));
      }

      for(std::size_t n = 0; n < parameterRotation_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(parameterRotation_(n)));
      }
    }

    serialisedOptimisationProblem.push_back(static_cast<double>(objectiveValueScaling_));
    serialisedOptimisationProblem.push_back(static_cast<double>(objectiveValueTranslation_));

    serialisedOptimisationProblem.push_back(static_cast<double>(acceptableObjectiveValue_));

    return serialisedOptimisationProblem;
  }

  template <typename T, typename U>
  void OptimisationProblem<T, U>::deserialise(
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
    
    if (std::is_floating_point<T>::value) {
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
    }

    objectiveValueScaling_ = static_cast<U>(serialisedOptimisationProblem.pop_back());
    objectiveValueTranslation_ = static_cast<U>(serialisedOptimisationProblem.pop_back());

    acceptableObjectiveValue_ = static_cast<U>(serialisedOptimisationProblem.pop_back());
  }
#endif
}
