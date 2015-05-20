namespace mant {
  template <typename T>
  class OptimisationProblem : public Printable {
    public:
      const unsigned int numberOfDimensions_;

      explicit OptimisationProblem(
        const unsigned int numberOfDimensions) noexcept;

      void setLowerBounds(
        const arma::Col<T>& lowerBounds);

      void setUpperBounds(
        const arma::Col<T>& upperBounds);

      arma::Col<T> getLowerBounds() const noexcept;

      arma::Col<T> getUpperBounds() const noexcept;

      double getSoftConstraintsValue(
        const arma::Col<T>& parameter);

      arma::Col<unsigned int> isOutOfLowerBounds(
        const arma::Col<T>& parameter);

      arma::Col<unsigned int> isOutOfUpperBounds(
        const arma::Col<T>& parameter);

      bool isSatisfyingSoftConstraints(
        const arma::Col<T>& parameter);

      bool isSatisfyingConstraints(
        const arma::Col<T>& parameter);

      void setParameterPermutation(
          const arma::Col<unsigned int>& parameterPermutation);

      void setParameterScaling(
        const arma::Col<double>& parameterScaling);

      void setParameterTranslation(
          const arma::Col<double>& parameterTranslation);

      void setParameterRotation(
        const arma::Mat<double>& parameterRotation);

      void setObjectiveValueScaling(
        const double objectiveValueScaling) noexcept;

      void setObjectiveValueTranslation(
        const double objectiveValueTranslation) noexcept;
      
      void setAcceptableObjectiveValue(
          const double acceptableObjectiveValue) noexcept;
        
      double getAcceptableObjectiveValue() const noexcept;

      double getObjectiveValue(
        const arma::Col<T>& parameter);

      unsigned int getNumberOfEvaluations() const noexcept;

      unsigned int getNumberOfDistinctEvaluations() const noexcept;

      void reset() noexcept;

      std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>> getCachedObjectiveValues() const noexcept;

      std::vector<long double> serialise() const noexcept;

      void deserialise(
          const std::vector<long double>& serialisedProblem);

      virtual ~OptimisationProblem() = default;

    protected:
      arma::Col<T> lowerBounds_;
      arma::Col<T> upperBounds_;

      arma::Col<unsigned int> parameterPermutation_;
      arma::Col<double> parameterScaling_;
      arma::Col<double> parameterTranslation_;
      arma::Mat<double> parameterRotation_;

      double objectiveValueScaling_;
      double objectiveValueTranslation_;

      double acceptableObjectiveValue_;

      unsigned int numberOfEvaluations_;
      unsigned int numberOfDistinctEvaluations_;

      arma::Col<T> getDiversifiedParameter(
        const arma::Col<T>& parameter) const noexcept;

      virtual double getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept;

      virtual double getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept = 0;

      std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>> cachedObjectiveValues_;
  };

  template <>
  inline OptimisationProblem<double>::OptimisationProblem(
      const unsigned int numberOfDimensions) noexcept;

  template <>
  inline void OptimisationProblem<double>::setParameterScaling(
      const arma::Col<double>& parameterScaling);

  template <>
  inline void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double>& parameterTranslation);

  template <>
  inline void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double>& parameterRotation);

  template <>
  inline arma::Col<double> OptimisationProblem<double>::getDiversifiedParameter(
      const arma::Col<double>& parameter) const noexcept;

  //
  // Implementation
  //

  template <typename T>
  OptimisationProblem<T>::OptimisationProblem(
      const unsigned int numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    // A vector with all elements set to the lowest representable value.
    setLowerBounds(arma::zeros<arma::Col<T>>(numberOfDimensions_) - std::numeric_limits<T>::max());
    // A vector with all elements set to the largest representable value.
    setUpperBounds(arma::zeros<arma::Col<T>>(numberOfDimensions_) + std::numeric_limits<T>::max());
    // (0, ..., numberOfDimensions - 1) 
    setParameterPermutation(arma::linspace<arma::Col<unsigned int>>(0, numberOfDimensions_ - 1, numberOfDimensions));
    setObjectiveValueScaling(1.0);
    setObjectiveValueTranslation(0.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  template <>
  inline OptimisationProblem<double>::OptimisationProblem(
      const unsigned int numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    // A vector with all elements set to the lowest representable value.
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - std::numeric_limits<double>::max());
    // A vector with all elements set to the largest representable value.
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + std::numeric_limits<double>::max());
    // (0, ..., numberOfDimensions - 1) 
    setParameterPermutation(arma::linspace<arma::Col<unsigned int>>(0, numberOfDimensions_ - 1, numberOfDimensions));
    setParameterScaling(arma::ones<arma::Col<double>>(numberOfDimensions_));
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_));
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

    const double& softConstraintValue = getSoftConstraintsValueImplementation(parameter);
    
    // All soft-constraint values must be greater than or equal to 0.
    assert(softConstraintValue >= 0);
    
    return objectiveValueScaling_ * softConstraintValue;
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationProblem<T>::isOutOfLowerBounds(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return parameter < lowerBounds_;
  }

  template <typename T>
  arma::Col<unsigned int> OptimisationProblem<T>::isOutOfUpperBounds(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return  parameter > upperBounds_;
  }

  template <typename T>
  bool OptimisationProblem<T>::isSatisfyingSoftConstraints(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return (getSoftConstraintsValue(parameter) == 0);
  }

  template <typename T>
  bool OptimisationProblem<T>::isSatisfyingConstraints(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    return (!arma::any(isOutOfLowerBounds(parameter)) && !arma::any(isOutOfUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));
  }

  template <typename T>
  inline void OptimisationProblem<T>::setParameterPermutation(
      const arma::Col<unsigned int>& parameterPermutation) {
    verify(parameterPermutation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions");
    verify(isPermutation(parameterPermutation, 0, numberOfDimensions_ - 1), "The parameter must be a permutation.");

    parameterPermutation_ = parameterPermutation;

    // Resets all counter and caches, as the problem was changed.
    reset();
  }

  template <>
  inline void OptimisationProblem<double>::setParameterScaling(
      const arma::Col<double>& parameterScaling) {
    verify(parameterScaling.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    parameterScaling_ = parameterScaling;

    // Resets all counter and caches, as the problem was changed.
    reset();
  }

  template <>
  inline void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    verify(parameterTranslation.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    parameterTranslation_ = parameterTranslation;

    // Resets all counter and caches, as the problem was changed.
    reset();
  }

  template <>
  inline void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double>& parameterRotation) {
    verify(parameterRotation.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions.");
    verify(isRotationMatrix(parameterRotation), "The parameter must be a rotation matrix.");

    parameterRotation_ = parameterRotation;

    // Resets all counter and caches, as the problem was changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) noexcept {
    objectiveValueScaling_ = objectiveValueScaling;

    // Resets all counter and caches, as the problem was changed.
    reset();
  }

  template <typename T>
  void OptimisationProblem<T>::setObjectiveValueTranslation(
      const double objectiveValueTranslation) noexcept {
    objectiveValueTranslation_ = objectiveValueTranslation;

    // Resets all counter and caches, as the problem was changed.
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
  inline double OptimisationProblem<T>::getObjectiveValue(
      const arma::Col<T>& parameter) {
    verify(parameter.n_elem == numberOfDimensions_, "The number of elements must be equal to the number of dimensions.");

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

    // Check if the result is already cached.
    const auto& cachePosition = cachedObjectiveValues_.find(parameter);
    if (cachePosition == cachedObjectiveValues_.cend()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      const double& result = objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      return cachePosition->second;
    }
  }

  template <typename T>
  unsigned int OptimisationProblem<T>::getNumberOfEvaluations() const noexcept {
    return numberOfEvaluations_;
  }

  template <typename T>
  unsigned int OptimisationProblem<T>::getNumberOfDistinctEvaluations() const noexcept {
    return numberOfDistinctEvaluations_;
  }

  template <typename T>
  void OptimisationProblem<T>::reset() noexcept {
    numberOfEvaluations_ = 0;
    numberOfDistinctEvaluations_ = 0;

    cachedObjectiveValues_.clear();
  }

  template <typename T>
  std::unordered_map<arma::Col<T>, double, Hash<T>, IsEqual<T>> OptimisationProblem<T>::getCachedObjectiveValues() const noexcept {
    return cachedObjectiveValues_;
  }

  template <typename T>
  inline arma::Col<T> OptimisationProblem<T>::getDiversifiedParameter(
      const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    // For discrete problems, the parameter is only permutated.
    return parameter.elem(parameterPermutation_);
  }

  template <>
  inline arma::Col<double> OptimisationProblem<double>::getDiversifiedParameter(
      const arma::Col<double>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    // The parameter is firstly permutated, than scaled, translated and lastly rotated.
    return parameterRotation_ * (parameterScaling_ % parameter.elem(parameterPermutation_) - parameterTranslation_);
  }

  template <typename T>
  double OptimisationProblem<T>::getSoftConstraintsValueImplementation(
      const arma::Col<T>& parameter) const noexcept {
    assert(parameter.n_elem == numberOfDimensions_);

    return 0.0;
  }

  template <typename T>
  std::vector<long double> OptimisationProblem<T>::serialise() const noexcept {
    std::vector<long double> serialisedProblem;

    serialisedProblem.push_back(numberOfDimensions_);

    for(std::size_t n = 0; n < lowerBounds_.n_elem; ++n) {
      serialisedProblem.push_back(lowerBounds_(n));
    }

    for(std::size_t n = 0; n < upperBounds_.n_elem; ++n) {
      serialisedProblem.push_back(upperBounds_(n));
    }

    for(std::size_t n = 0; n < parameterPermutation_.n_elem; ++n) {
      serialisedProblem.push_back(parameterPermutation_(n));
    }

    for(std::size_t n = 0; n < parameterScaling_.n_elem; ++n) {
      serialisedProblem.push_back(parameterScaling_(n));
    }

    for(std::size_t n = 0; n < parameterTranslation_.n_elem; ++n) {
      serialisedProblem.push_back(parameterTranslation_(n));
    }

    for(std::size_t n = 0; n < parameterRotation_.n_elem; ++n) {
      serialisedProblem.push_back(parameterRotation_(n));
    }

    serialisedProblem.push_back(objectiveValueScaling_);
    serialisedProblem.push_back(objectiveValueTranslation_);

    serialisedProblem.push_back(acceptableObjectiveValue_);
  }

  template <typename T>
  void OptimisationProblem<T>::deserialise(
      const std::vector<long double>& serialisedProblem) {

  }
}
