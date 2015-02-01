namespace mant {
  template <typename ParameterType>
  class PropertiesEstimation {
    public:
      explicit PropertiesEstimation(
          const std::shared_ptr<CorrelationAnalysis<ParameterType>> correlationAnalysis,
          const std::shared_ptr<LipschitzContinuityAnalysis<ParameterType>> lipschitzContinuityAnalysis,
          const std::shared_ptr<LinearModelAnalysis<ParameterType>> linearModelAnalysis,
          const std::shared_ptr<QuadraticModelAnalysis<ParameterType>> quadraticModelAnalysis,
          const std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType>> additiveSeparabilityAnalysis) noexcept;

      void estimate(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept;

      std::size_t getNumberOfPropertySets() const noexcept;

      double getCorrelationCoefficient(
          const std::size_t& propertiesSetIndex) const;

      bool isLinear(
          const std::size_t& propertiesSetIndex) const;
      arma::Col<double> getLinearModelEstimator(
          const std::size_t& propertiesSetIndex) const;

      bool isQuadratic(
          const std::size_t& propertiesSetIndex) const;
      arma::Col<double> getQuadraticModelEstimator(
          const std::size_t& propertiesSetIndex) const;

      double getLipschitzConstant(
          const std::size_t& propertiesSetIndex) const;

      bool isSeparable(
          const std::size_t& propertiesSetIndex) const;
      std::vector<arma::Col<unsigned int>> getSeparations(
          const std::size_t& propertiesSetIndex) const;

      double getPlausibility(
          const std::size_t& propertiesSetIndex) const;

      // Provides a default deconstructor.
      virtual ~OptimisationProblem() = default;

    protected:
      std::shared_ptr<CorrelationAnalysis<ParameterType>> correlationAnalysis_;
      std::shared_ptr<LipschitzContinuityAnalysis<ParameterType>> lipschitzContinuityAnalysis_;
      std::shared_ptr<LinearModelAnalysis<ParameterType>> linearModelAnalysis_;
      std::shared_ptr<QuadraticModelAnalysis<ParameterType>> quadraticModelAnalysis_;
      std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType>> additiveSeparabilityAnalysis_;

      std::size_t numberOfPropertySets_;

      std::vector<bool> isLinear_;
      std::vector<arma::Col<double>> linearModelEstimators_;

      std::vector<bool> isQuadratic_;
      std::vector<arma::Col<double>> quadraticModelEstimators_;

      std::vector<double> lipschitzConstants_;

      std::vector<double> correlationCoefficients_;

      std::vector<bool> isSeparable_;
      std::vector<std::vector<arma::Col<unsigned int>>> separations_;

//      std::vector<bool> isProportionalSeparable_;
//      std::vector<std::unordered_map<arma::Col<unsigned int>, std::pair<arma::Col<unsigned int>, double>, Hasher, KeyEqual>> proportionalPartitions_;

      std::vector<double> plausibilities_;

      virtual void estimateImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept = 0;
  };

  //
  // Implementation
  //

  template <typename ParameterType>
  PropertiesEstimation<ParameterType>::PropertiesEstimation(
      const std::shared_ptr<CorrelationAnalysis<ParameterType>> correlationAnalysis,
      const std::shared_ptr<LipschitzContinuityAnalysis<ParameterType>> lipschitzContinuityAnalysis,
      const std::shared_ptr<LinearModelAnalysis<ParameterType>> linearModelAnalysis,
      const std::shared_ptr<QuadraticModelAnalysis<ParameterType>> quadraticModelAnalysis,
      const std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType>> additiveSeparabilityAnalysis) noexcept
    : correlationAnalysis_(correlationAnalysis),
      lipschitzContinuityAnalysis_(lipschitzContinuityAnalysis),
      linearModelAnalysis_(linearModelAnalysis),
      quadraticModelAnalysis_(quadraticModelAnalysis),
      additiveSeparabilityAnalysis_(additiveSeparabilityAnalysis),
      numberOfPropertySets_(0),
      isLinear_(false),
      isQuadratic_(false) {

  }

  template <typename ParameterType>
  void PropertiesEstimation<ParameterType>::estimate(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    return estimateImplementation(optimisationProblem);
  }

  template <typename ParameterType>
  std::size_t PropertiesEstimation<ParameterType>::getNumberOfPropertySets() const noexcept {
    return numberOfPropertySets_;
  }

  template <typename ParameterType>
  double PropertiesEstimation<ParameterType>::getCorrelationCoefficient(
      const std::size_t& propertiesSetIndex) const {
    return correlationCoefficients_.at(propertiesSetIndex);
  }

  template <typename ParameterType>
  bool PropertiesEstimation<ParameterType>::isLinear(
      const std::size_t& propertiesSetIndex) const {
    return isLinear_.at(propertiesSetIndex);
  }

  template <typename ParameterType>
  arma::Col<double> PropertiesEstimation<ParameterType>::getLinearModelEstimator(
      const std::size_t& propertiesSetIndex) const {
    return linearModelEstimators_.at(propertiesSetIndex);
  }

  template <typename ParameterType>
  bool PropertiesEstimation<ParameterType>::isQuadratic(
      const std::size_t& propertiesSetIndex) const {
    return isQuadratic_.at(propertiesSetIndex);
  }

  template <typename ParameterType>
  arma::Col<double> PropertiesEstimation<ParameterType>::getQuadraticModelEstimator(
      const std::size_t& propertiesSetIndex) const {
    return quadraticModelEstimators_.at(propertiesSetIndex);
  }

  template <typename ParameterType>
  double PropertiesEstimation<ParameterType>::getLipschitzConstant(
      const std::size_t& propertiesSetIndex) const {
    return lipschitzConstants_.at(propertiesSetIndex);
  }

  template <typename ParameterType>
  bool PropertiesEstimation<ParameterType>::isSeparable(
      const std::size_t& propertiesSetIndex) const {
    return isSeparable_.at(propertiesSetIndex);
  }
}
