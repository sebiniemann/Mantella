namespace mant {
  template <typename ParameterType>
  class OptimisationProblem : public Printable {
    public:
      const unsigned int numberOfDimensions_;
.
      explicit OptimisationProblem(
        const unsigned int& numberOfDimensions) noexcept;

      arma::Col<unsigned int> isSatisfyingLowerBounds(
        const arma::Col<ParameterType>& parameter);

      arma::Col<unsigned int> isSatisfyingUpperBounds(
        const arma::Col<ParameterType>& parameter);

      bool isSatisfyingSoftConstraints(
        const arma::Col<ParameterType>& parameter);

      bool isSatisfyingConstraints(
        const arma::Col<ParameterType>& parameter);

      double getSoftConstraintsValue(
        const arma::Col<ParameterType>& parameter);

      double getObjectiveValue(
        const arma::Col<ParameterType>& parameter);

      arma::Col<ParameterType> getLowerBounds() const noexcept;

      void setLowerBounds(
        const arma::Col<ParameterType> lowerBounds);

      arma::Col<ParameterType> getUpperBounds() const noexcept;

      void setUpperBounds(
        const arma::Col<ParameterType> upperBounds);

      void setParameterTranslation(
          const arma::Col<ParameterType> parameterTranslation);

      void setParameterScale(
        const arma::Col<ParameterType> parameterScale);

      void setParameterRotation(
        const arma::Mat<ParameterType> parameterRotation);

      void setObjectiveValueTranslation(
        const double objectiveValueTranslation) noexcept;

      void setObjectiveValueScale(
        const double objectiveValueScale) noexcept;
        
      double getAcceptableObjectiveValue() const noexcept;
      
      void setAcceptableObjectiveValue(
          const double acceptableObjectiveValue) noexcept;

      unsigned int getNumberOfEvaluations() const noexcept;

      unsigned int getNumberOfDistinctEvaluations() const noexcept;

      void reset() noexcept;

      std::unordered_map<arma::Col<ParameterType>, double, Hash, IsEqual> getCachedObjectiveValues() const noexcept;

      virtual ~OptimisationProblem() = default;

    protected:
      arma::Col<ParameterType> lowerBounds_;
      arma::Col<ParameterType> upperBounds_;

      arma::Col<double> parameterTranslation_;
      arma::Col<double> parameterScale_;
      arma::Mat<double> parameterRotation_;

      double objectiveValueTranslation_;
      double objectiveValueScale_;

      double acceptableObjectiveValue_;

      unsigned int numberOfEvaluations_;
      unsigned int numberOfDistinctEvaluations_;

      arma::Col<ParameterType> getScaledCongruentParameter(
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
        archive(cereal::make_nvp("parameterTranslation", parameterTranslation_));
        archive(cereal::make_nvp("parameterRotation", parameterRotation_));
        archive(cereal::make_nvp("parameterScale", parameterScale_));
        archive(cereal::make_nvp("objectiveValueTranslation", objectiveValueTranslation_));
        archive(cereal::make_nvp("objectiveValueScale", objectiveValueScale_));
        archive(cereal::make_nvp("acceptableObjectiveValue", acceptableObjectiveValue_));
      }
#endif
  };

  //
  // Implementation
  //

  template <>
  inline OptimisationProblem<double>::OptimisationProblem(
      const unsigned int& numberOfDimensions) noexcept;

  template <>
  inline void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double> parameterTranslation);

  template <>
  inline void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double> parameterRotation);

  template <>
  inline void OptimisationProblem<double>::setParameterScale(
      const arma::Col<double> parameterScale);

  template <>
  inline arma::Col<double> OptimisationProblem<double>::getScaledCongruentParameter(
      const arma::Col<double>& parameter) const noexcept;

  template <typename ParameterType>
  OptimisationProblem<ParameterType>::OptimisationProblem(
      const unsigned int& numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    setLowerBounds(arma::zeros<arma::Col<ParameterType>>(numberOfDimensions_) - std::numeric_limits<ParameterType>::max());
    setUpperBounds(arma::zeros<arma::Col<ParameterType>>(numberOfDimensions_) + std::numeric_limits<ParameterType>::max());
    setObjectiveValueTranslation(0.0);
    setObjectiveValueScale(1.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  template <>
  inline OptimisationProblem<double>::OptimisationProblem(
      const unsigned int& numberOfDimensions) noexcept
    : numberOfDimensions_(numberOfDimensions),
      numberOfEvaluations_(0),
      numberOfDistinctEvaluations_(0) {
    setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - std::numeric_limits<double>::max());
    setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + std::numeric_limits<double>::max());
    setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_));
    setParameterRotation(arma::eye<arma::Mat<double>>(numberOfDimensions_, numberOfDimensions_));
    setParameterScale(arma::ones<arma::Col<double>>(numberOfDimensions_));
    setObjectiveValueTranslation(0.0);
    setObjectiveValueScale(1.0);
    setAcceptableObjectiveValue(std::numeric_limits<double>::lowest());
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationProblem<ParameterType>::isSatisfyingLowerBounds(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    return (parameter >= lowerBounds_);
  }

  template <typename ParameterType>
  arma::Col<unsigned int> OptimisationProblem<ParameterType>::isSatisfyingUpperBounds(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    return (parameter <= upperBounds_);
  }

  template <typename ParameterType>
  bool OptimisationProblem<ParameterType>::isSatisfyingSoftConstraints(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    return (getSoftConstraintsValue(parameter) == 0);
  }

  template <typename ParameterType>
  bool OptimisationProblem<ParameterType>::isSatisfyingConstraints(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }
    
    return (arma::all(isSatisfyingLowerBounds(parameter)) && arma::all(isSatisfyingUpperBounds(parameter)) && isSatisfyingSoftConstraints(parameter));
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getSoftConstraintsValue(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    return getSoftConstraintsValueImplementation(parameter);
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getObjectiveValue(
      const arma::Col<ParameterType>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Always increase the number of evaluations (whether its computed or retrived from cache).
    ++numberOfEvaluations_;

    // Check if the result is already cached.
    const auto& cachePosition = cachedObjectiveValues_.find(parameter);
    if (cachePosition == cachedObjectiveValues_.cend()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      // The result was not found, compute it.
      const double& result = objectiveValueScale_ * getObjectiveValueImplementation(parameter) + objectiveValueTranslation_;
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <>
  inline double OptimisationProblem<double>::getObjectiveValue(
      const arma::Col<double>& parameter) {
    if (parameter.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter (" + std::to_string(parameter.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    // Always increase the number of evaluations (whether its computed or retrived from cache).
    ++numberOfEvaluations_;

    // Check if the result is already cached.
    const auto& cachePosition = cachedObjectiveValues_.find(parameter);
    if (cachePosition == cachedObjectiveValues_.cend()) {
      // Increase the number of distinct evaluations only if we actually compute the value.
      ++numberOfDistinctEvaluations_;

      // The result was not found, compute it.
      const double& result = objectiveValueScale_ * getObjectiveValueImplementation(getScaledCongruentParameter(parameter)) + objectiveValueTranslation_;
      cachedObjectiveValues_.insert({parameter, result});
      return result;
    } else {
      // Return the found result.
      return cachePosition->second;
    }
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationProblem<ParameterType>::getLowerBounds() const noexcept {
    return lowerBounds_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setLowerBounds(
      const arma::Col<ParameterType> lowerBounds) {
    if (lowerBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the lower bound (" + std::to_string(lowerBounds.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    lowerBounds_ = lowerBounds;
  }

  template <typename ParameterType>
  arma::Col<ParameterType> OptimisationProblem<ParameterType>::getUpperBounds() const noexcept {
    return upperBounds_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setUpperBounds(
      const arma::Col<ParameterType> upperBounds) {
    if (upperBounds.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the upper bound (" + std::to_string(upperBounds.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    upperBounds_ = upperBounds;
  }

  template <>
  inline void OptimisationProblem<double>::setParameterTranslation(
      const arma::Col<double> parameterTranslation) {
    if (parameterTranslation.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter translation (" + std::to_string(parameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterTranslation_ = parameterTranslation;
  }

  template <>
  inline void OptimisationProblem<double>::setParameterRotation(
      const arma::Mat<double> parameterRotation) {
    if (!parameterRotation.is_square()) {
      throw std::logic_error("The rotation matrix (" + std::to_string(parameterRotation.n_rows) + ", " + std::to_string(parameterRotation.n_cols) + ") must be square.");
    } else if (parameterRotation.n_rows != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(parameterRotation.n_rows) + ", " + std::to_string(parameterRotation.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    } else if(arma::any(arma::vectorise(arma::abs(parameterRotation.i() - parameterRotation.t()) > 1.0e-12 * arma::median(arma::vectorise(parameterRotation))))) {
      throw std::logic_error("The rotation matrix must be orthonormal.");
    } else if(std::abs(std::abs(arma::det(parameterRotation)) - 1.0) > 1.0e-12) {
      throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(parameterRotation)) + ") must be either 1 or -1.");
    }

    parameterRotation_ = parameterRotation;
  }

  template <>
  inline void OptimisationProblem<double>::setParameterScale(
      const arma::Col<double> parameterScale) {
    if (parameterScale.n_elem != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of the parameter scale (" + std::to_string(parameterScale.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    }

    parameterScale_ = parameterScale;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setObjectiveValueTranslation(
      const double objectiveValueTranslation) noexcept {
    objectiveValueTranslation_ = objectiveValueTranslation;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::OptimisationProblem::setObjectiveValueScale(
      const double objectiveValueScale) noexcept {
    objectiveValueScale_ = objectiveValueScale;
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getAcceptableObjectiveValue() const noexcept {
    return acceptableObjectiveValue_;
  }

  template <typename ParameterType>
  void OptimisationProblem<ParameterType>::setAcceptableObjectiveValue(
      const double acceptableObjectiveValue) noexcept {
    acceptableObjectiveValue_ = acceptableObjectiveValue;
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
    cachedSoftConstraintsValues_.clear();
    cachedIsSatisfyingLowerBounds_.clear();
    cachedIsSatisfyingUpperBounds_.clear();
    cachedIsSatisfyingSoftConstraints_.clear();
    cachedIsSatisfyingConstraints_.clear();
  }

  template <typename ParameterType>
  std::unordered_map<arma::Col<ParameterType>, double, Hash, IsEqual> OptimisationProblem<ParameterType>::getCachedObjectiveValues() const noexcept {
    return cachedObjectiveValues_;
  }

  template <>
  inline arma::Col<double> OptimisationProblem<double>::getScaledCongruentParameter(
      const arma::Col<double>& parameter) const noexcept {
    return parameterRotation_ * (parameter + (parameterScale_ % parameterTranslation_));
  }

  template <typename ParameterType>
  double OptimisationProblem<ParameterType>::getSoftConstraintsValueImplementation(
      const arma::Col<ParameterType>& parameter) const noexcept {
    return 0.0;
  }
}
