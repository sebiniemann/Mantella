#include <hop_bits/propertiesEstimation.hpp>

namespace hop {
  PropertiesEstimation::PropertiesEstimation()
    : isLinear_(false),
      isQuadratic_(false),
      isLipschitzContinuous_(false),
      isAlphaHoelderContinuous_(false),
      numberOfPropertySets_(0) {

  }

  void PropertiesEstimation::addPropertiesAnalysis(
      const std::shared_ptr<PropertiesAnalysis> propertiesAnalysis) {
    propertiesAnalyses_.push_back(propertiesAnalysis);
  }

  std::size_t PropertiesEstimation::getNumberOfPropertySets() const {
    return numberOfPropertySets_;
  }

  bool PropertiesEstimation::isLinear(
      const std::size_t& propertiesSetIndex) const {
    return isLinear_.at(propertiesSetIndex);
  }

  arma::Col<double> PropertiesEstimation::getLinearEstimator(
      const std::size_t& propertiesSetIndex) const {
    return linearEstimator_.at(propertiesSetIndex);
  }

  bool PropertiesEstimation::isQuadratic(
      const std::size_t& propertiesSetIndex) const{
    return isQuadratic_.at(propertiesSetIndex);
  }

  arma::Col<double> PropertiesEstimation::getQuadraticEstimator(
      const std::size_t& propertiesSetIndex) const{
    return quadraticEstimator_.at(propertiesSetIndex);
  }

  bool PropertiesEstimation::isLipschitzContinuous(
      const std::size_t& propertiesSetIndex) const {
    return isLipschitzContinuous_.at(propertiesSetIndex);
  }

  double PropertiesEstimation::getLipschitzConstant(
      const std::size_t& propertiesSetIndex) const {
    return lipschitzConstant_.at(propertiesSetIndex);
  }

  bool PropertiesEstimation::isAlphaHoelderContinuous(
      const std::size_t& propertiesSetIndex) const {
    return isAlphaHoelderContinuous_.at(propertiesSetIndex);
  }

  double PropertiesEstimation::getAlphaHoelderConstant(
      const std::size_t& propertiesSetIndex) const {
    return alphaHoelderConstant_.at(propertiesSetIndex);
  }
}
