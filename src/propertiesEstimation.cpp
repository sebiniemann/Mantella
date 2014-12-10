#include <hop_bits/propertiesEstimation.hpp>

namespace hop {
  PropertiesEstimation::PropertiesEstimation(
      const std::shared_ptr<CorrelationAnalysis> correlationAnalysis,
      const std::shared_ptr<LinearModelAnalysis> linearModelAnalysis,
      const std::shared_ptr<QuadraticModelAnalysis> quadraticModelAnalysis,
      const std::shared_ptr<LipschitzContinuityAnalysis> lipschitzContinuityAnalysis,
      const std::shared_ptr<AdditiveSeparabilityAnalysis> additiveSeparabilityAnalysis) noexcept
    : correlationAnalysis_(correlationAnalysis),
      linearModelAnalysis_(linearModelAnalysis),
      quadraticModelAnalysis_(quadraticModelAnalysis),
      lipschitzContinuityAnalysis_(lipschitzContinuityAnalysis),
      additiveSeparabilityAnalysis_(additiveSeparabilityAnalysis),
      numberOfPropertySets_(0),
      isLinear_(false),
      isQuadratic_(false) {

  }

  void PropertiesEstimation::estimate(
      const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept {
    return estimateImplementation(optimisationProblem);
  }

  std::size_t PropertiesEstimation::getNumberOfPropertySets() const noexcept {
    return numberOfPropertySets_;
  }

  double PropertiesEstimation::getCorrelationCoefficient(
      const std::size_t& propertiesSetIndex) const {
    return correlationCoefficient_.at(propertiesSetIndex);
  }

  bool PropertiesEstimation::isLinear(
      const std::size_t& propertiesSetIndex) const {
    return isLinear_.at(propertiesSetIndex);
  }

  arma::Col<double> PropertiesEstimation::getLinearModelEstimator(
      const std::size_t& propertiesSetIndex) const {
    return linearModelEstimators_.at(propertiesSetIndex);
  }

  bool PropertiesEstimation::isQuadratic(
      const std::size_t& propertiesSetIndex) const{
    return isQuadratic_.at(propertiesSetIndex);
  }

  arma::Col<double> PropertiesEstimation::getQuadraticModelEstimator(
      const std::size_t& propertiesSetIndex) const{
    return quadraticModelEstimators_.at(propertiesSetIndex);
  }

  double PropertiesEstimation::getLipschitzConstant(
      const std::size_t& propertiesSetIndex) const {
    return lipschitzConstants_.at(propertiesSetIndex);
  }

  bool PropertiesEstimation::isSeparable(
      const std::size_t& propertiesSetIndex) const {
    return isSeparable_.at(propertiesSetIndex);
  }

  std::vector<arma::Col<arma::uword>> PropertiesEstimation::getPartitions(
      const std::size_t& propertiesSetIndex) const {
    return partitions_.at(propertiesSetIndex);
  }
}
