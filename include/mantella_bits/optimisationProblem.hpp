namespace mant {
  template <typename ParameterType>
  class OptimisationProblem : public Printable {
    public:
      const unsigned int numberOfDimensions_;

      explicit OptimisationProblem(
        const unsigned int numberOfDimensions) noexcept;

      void setLowerBounds(
        const arma::Col<ParameterType>& lowerBounds);

      void setUpperBounds(
        const arma::Col<ParameterType>& upperBounds);

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
        const arma::Col<ParameterType>& parameter);

      unsigned int getNumberOfEvaluations() const noexcept;

      unsigned int getNumberOfDistinctEvaluations() const noexcept;

      void reset() noexcept;

      std::unordered_map<arma::Col<ParameterType>, double, Hash, IsEqual> getCachedObjectiveValues() const noexcept;

      virtual ~OptimisationProblem() = default;

    protected:
      arma::Col<ParameterType> lowerBounds_;
      arma::Col<ParameterType> upperBounds_;

      arma::Col<unsigned int> parameterPermutation_;
      arma::Col<double> parameterScaling_;
      arma::Col<double> parameterTranslation_;
      arma::Mat<double> parameterRotation_;

      double objectiveValueScaling_;
      double objectiveValueTranslation_;

      double acceptableObjectiveValue_;

      unsigned int numberOfEvaluations_;
      unsigned int numberOfDistinctEvaluations_;

      arma::Col<ParameterType> getDiversifiedParameter(
        const arma::Col<ParameterType>& parameter) const noexcept;

      virtual double getSoftConstraintsValueImplementation(
        const arma::Col<ParameterType>& parameter) const noexcept;

      virtual double getObjectiveValueImplementation(
        const arma::Col<ParameterType>& parameter) const noexcept = 0;

      std::unordered_map<arma::Col<ParameterType>, double, Hash, IsEqual> cachedObjectiveValues_;

 #if defined(MANTELLA_USE_PARALLEL)
      friend class cereal::access;
      OptimisationProblem() = default;
      
      template <typename Archive>
      void serialize(
          Archive& archive) noexcept {
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("lowerBounds", lowerBounds_));
        archive(cereal::make_nvp("upperBounds", upperBounds_));
        archive(cereal::make_nvp("parameterPermutation", parameterPermutation_));
        archive(cereal::make_nvp("parameterScaling", parameterScaling_));
        archive(cereal::make_nvp("parameterTranslation", parameterTranslation_));
        archive(cereal::make_nvp("parameterRotation", parameterRotation_));
        archive(cereal::make_nvp("objectiveValueScaling", objectiveValueScaling_));
        archive(cereal::make_nvp("objectiveValueTranslation", objectiveValueTranslation_));
        archive(cereal::make_nvp("acceptableObjectiveValue", acceptableObjectiveValue_));
      }
#endif
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

  template <typename ParameterType>
  OptimisationProblem<ParameterType>::OptimisationProblem(
      const unsigned int numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    // A vector with all elements set to the lowest representable value.
    setLowerBounds(arma::zeros<arma::Col<ParameterType>>(numberOfDimensions_) - std::numeric_limits<ParameterType>::max());
    // A vector with all elements set to the largest representable value.
    setUpperBounds(arma::zeros<arma::Col<ParameterType>>(numberOfDimensions_) + std::numeric_limits<ParameterType>::max());
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

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setLowerBounds(
      const arma::Col<ParameterType>& lowerBounds) {
    isEqual("The number of elements", lowerBounds.n_elem, "the number of dimensions", numberOfDimensions_);

    lowerBounds_ = lowerBounds;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setUpperBounds(
      const arma::Col<ParameterType>& upperBounds) {
    isEqual("The number of elements", upperBounds.n_elem, "the number of dimensions", numberOfDimensions_);

    upperBounds_ = upperBounds;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationProblem<ParameterType>::getLowerBounds() const noexcept {
    return lowerBounds_;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationProblem<ParameterType>::getUpperBounds() const noexcept {
    return upperBounds_;
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getSoftConstraintsValue(
      const arma::Col<ParameterType>& parameter) {
    isEqual("The number of elements", parameter.n_elem, "the number of dimensions", numberOfDimensions_);

    const double& softConstraintValue = getSoftConstraintsValueImplementation(parameter);
    
    // All soft-constraint values must be greater than or equal to 0.
    assert(softConstraintValue >= 0);
    
    return objectiveValueScaling_ * softConstraintValue;
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationProblem<ParameterType>::isSatisfyingLowerBounds(
      const arma::Col<ParameterType>& parameter) {
    isEqual("The number of elements", parameter.n_elem, "the number of dimensions", numberOfDimensions_);

    return arma::all(parameter >= lowerBounds_);
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationProblem<ParameterType>::isSatisfyingUpperBounds(
      const arma::Col<ParameterType>& parameter) {
    isEqual("The number of elements", parameter.n_elem, "the number of dimensions", numberOfDimensions_);

    return  arma::all(parameter <= upperBounds_);
  }

  template <typename ParameterType>
  bool OptimisationProblem<ParameterType>::isSatisfyingSoftConstraints(
      const arma::Col<ParameterType>& parameter) {
    isEqual("The number of elements", parameter.n_elem, "the number of dimensions", numberOfDimensions_);

    return (getSoftConstraintsValue(parameter) == 0);
  }

  template <typename ParameterType>
  bool OptimisationProblem<ParameterType>::isSatisfyingConstraints(
      const arma::Col<ParameterType>& parameter) {
    isEqual("The number of elements", parameter.n_elem, "the number of dimensions", numberOfDimensions_);
    
    // Returns only true, if all other isSatisfying*-methods will also return true.
    return (arma::all(isSatisfyingLowerBounds(parameter)) && arma::all(isSatisfyingUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));
  }

  template <typename ParameterType>
  inline void OptimisationProblem<ParameterType>::setParameterPermutation(
      const arma::Col<unsigned int>& parameterPermutation) {
    //isPermutation("The vector", parameterPermutation, 0, parameterPermutation.n_elem - 1); // TODO implement. Checks uniqueness and bounds
    isEqual("The number of elements", parameterPermutation.n_elem, "the number of dimensions", numberOfDimensions_);

    parameterPermutation_ = parameterPermutation;
  }

  template <>
  inline void OptimisationProblem<double>::setParameterScaling(
      const arma::Col<double>& parameterScaling) {
    isEqual("The number of elements", parameterScaling.n_elem, "the number of dimensions", numberOfDimensions_);

    parameterScaling_ = parameterScaling;
  }

  template <>
  inline void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double>& parameterTranslation) {
    isEqual("The number of elements", parameterTranslation.n_elem, "the number of dimensions", numberOfDimensions_);

    parameterTranslation_ = parameterTranslation;
  }

  template <>
  inline void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double>& parameterRotation) {
    isEqual("The number of rows", parameterRotation.n_rows, "the number of dimensions", numberOfDimensions_);
    isRotationMatrix("The matrix", parameterRotation);

    parameterRotation_ = parameterRotation;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::OptimisationProblem::setObjectiveValueScaling(
      const double objectiveValueScaling) noexcept {
    objectiveValueScaling_ = objectiveValueScaling;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setObjectiveValueTranslation(
      const double objectiveValueTranslation) noexcept {
    objectiveValueTranslation_ = objectiveValueTranslation;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setAcceptableObjectiveValue(
      const double acceptableObjectiveValue) noexcept {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getAcceptableObjectiveValue() const noexcept {
    return acceptableObjectiveValue_;
  }

  template <typename ParameterType>
  inline double OptimisationProblem<ParameterType>::getObjectiveValue(
      const arma::Col<ParameterType>& parameter) {
    isEqual("The number of elements", parameter.n_elem, "the number of dimensions", numberOfDimensions_);

    // Always increase the number of evaluations.
    ++numberOfEvaluations_;

    // Check if the result is already cached.
    const auto& cachePosition = cachedObjectiveValues_.find(parameter);
    if (cachePosition == cachedObjectiveValues_.cend()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      // The result was not found, compute it.
      const double& result = objectiveValueScaling_ * getObjectiveValueImplementation(getDiversifiedParameter(parameter)) + objectiveValueTranslation_;
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <typename ParameterType>
  unsigned int OptimisationProblem<ParameterType>::getNumberOfEvaluations() const noexcept {
    return numberOfEvaluations_;
  }

  template <typename ParameterType>
  unsigned int OptimisationProblem<ParameterType>::getNumberOfDistinctEvaluations() const noexcept {
    return numberOfDistinctEvaluations_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::reset() noexcept {
    numberOfEvaluations_ = 0;
    numberOfDistinctEvaluations_ = 0;

    cachedObjectiveValues_.clear();
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, double, Hash, IsEqual> OptimisationProblem<ParameterType>::getCachedObjectiveValues() const noexcept {
    return cachedObjectiveValues_;
  }

  template <typename ParameterType>
  inline arma::Col<ParameterType> OptimisationProblem<ParameterType>::getDiversifiedParameter(
      const arma::Col<ParameterType>& parameter) const noexcept {
    assert(isEqual(parameter.n_elem, numberOfDimensions_));

    // The parameter is only permutated for non-continuous problems.
    return parameter.elem(parameterPermutation_);
  }

  template <>
  inline arma::Col<double> OptimisationProblem<double>::getDiversifiedParameter(
      const arma::Col<double>& parameter) const noexcept {
    assert(isEqual(parameter.n_elem, numberOfDimensions_));

    // The paraemter is firstly permutated, than scaled, translated and lastly rotated.
    return parameterRotation_ * (parameterScaling_ % parameter.elem(parameterPermutation_) - parameterTranslation_);
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getSoftConstraintsValueImplementation(
      const arma::Col<ParameterType>& parameter) const noexcept {
    assert(isEqual(parameter.n_elem, numberOfDimensions_));

    return 0.0;
  }
}
