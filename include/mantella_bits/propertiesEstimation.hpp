namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class PropertiesEstimation {
    public:
      explicit PropertiesEstimation(
          const std::shared_ptr<CorrelationAnalysis<ParameterType, DistanceFunction>> correlationAnalysis,
          const std::shared_ptr<LipschitzContinuityAnalysis<ParameterType, DistanceFunction>> lipschitzContinuityAnalysis,
          const std::shared_ptr<LinearModelAnalysis<ParameterType, DistanceFunction>> linearModelAnalysis,
          const std::shared_ptr<QuadraticModelAnalysis<ParameterType, DistanceFunction>> quadraticModelAnalysis,
          const std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>> additiveSeparabilityAnalysis) noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesEstimation(const PropertiesEstimation&) = delete;
      PropertiesEstimation& operator=(const PropertiesEstimation&) = delete;

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

    protected:
      std::shared_ptr<CorrelationAnalysis<ParameterType, DistanceFunction>> correlationAnalysis_;
      std::shared_ptr<LipschitzContinuityAnalysis<ParameterType, DistanceFunction>> lipschitzContinuityAnalysis_;
      std::shared_ptr<LinearModelAnalysis<ParameterType, DistanceFunction>> linearModelAnalysis_;
      std::shared_ptr<QuadraticModelAnalysis<ParameterType, DistanceFunction>> quadraticModelAnalysis_;
      std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>> additiveSeparabilityAnalysis_;

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

  template <typename ParameterType, class DistanceFunction>
  PropertiesEstimation<ParameterType, DistanceFunction>::PropertiesEstimation(
      const std::shared_ptr<CorrelationAnalysis<ParameterType, DistanceFunction>> correlationAnalysis,
      const std::shared_ptr<LipschitzContinuityAnalysis<ParameterType, DistanceFunction>> lipschitzContinuityAnalysis,
      const std::shared_ptr<LinearModelAnalysis<ParameterType, DistanceFunction>> linearModelAnalysis,
      const std::shared_ptr<QuadraticModelAnalysis<ParameterType, DistanceFunction>> quadraticModelAnalysis,
      const std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType, DistanceFunction>> additiveSeparabilityAnalysis) noexcept
    : correlationAnalysis_(correlationAnalysis),
      lipschitzContinuityAnalysis_(lipschitzContinuityAnalysis),
      linearModelAnalysis_(linearModelAnalysis),
      quadraticModelAnalysis_(quadraticModelAnalysis),
      additiveSeparabilityAnalysis_(additiveSeparabilityAnalysis),
      numberOfPropertySets_(0),
      isLinear_(false),
      isQuadratic_(false) {

  }

  template <typename ParameterType, class DistanceFunction>
  void PropertiesEstimation<ParameterType, DistanceFunction>::estimate(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    return estimateImplementation(optimisationProblem);
  }

  template <typename ParameterType, class DistanceFunction>
  std::size_t PropertiesEstimation<ParameterType, DistanceFunction>::getNumberOfPropertySets() const noexcept {
    return numberOfPropertySets_;
  }

  template <typename ParameterType, class DistanceFunction>
  double PropertiesEstimation<ParameterType, DistanceFunction>::getCorrelationCoefficient(
      const std::size_t& propertiesSetIndex) const {
    return correlationCoefficients_.at(propertiesSetIndex);
  }

  template <typename ParameterType, class DistanceFunction>
  bool PropertiesEstimation<ParameterType, DistanceFunction>::isLinear(
      const std::size_t& propertiesSetIndex) const {
    return isLinear_.at(propertiesSetIndex);
  }

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> PropertiesEstimation<ParameterType, DistanceFunction>::getLinearModelEstimator(
      const std::size_t& propertiesSetIndex) const {
    return linearModelEstimators_.at(propertiesSetIndex);
  }

  template <typename ParameterType, class DistanceFunction>
  bool PropertiesEstimation<ParameterType, DistanceFunction>::isQuadratic(
      const std::size_t& propertiesSetIndex) const {
    return isQuadratic_.at(propertiesSetIndex);
  }

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> PropertiesEstimation<ParameterType, DistanceFunction>::getQuadraticModelEstimator(
      const std::size_t& propertiesSetIndex) const {
    return quadraticModelEstimators_.at(propertiesSetIndex);
  }

  template <typename ParameterType, class DistanceFunction>
  double PropertiesEstimation<ParameterType, DistanceFunction>::getLipschitzConstant(
      const std::size_t& propertiesSetIndex) const {
    return lipschitzConstants_.at(propertiesSetIndex);
  }

  template <typename ParameterType, class DistanceFunction>
  bool PropertiesEstimation<ParameterType, DistanceFunction>::isSeparable(
      const std::size_t& propertiesSetIndex) const {
    return isSeparable_.at(propertiesSetIndex);
  }
}
