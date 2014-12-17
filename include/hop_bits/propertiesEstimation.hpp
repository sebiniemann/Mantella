#pragma once

// C++ Standard Library
#include <memory>
#include <vector>

// HOP
#include <hop_bits/propertiesEstimation.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp>

namespace hop {
  template <typename ParameterType>
  class PropertiesEstimation {
    public:
      explicit PropertiesEstimation(
          const std::shared_ptr<CorrelationAnalysis<ParameterType>> correlationAnalysis,
          const std::shared_ptr<LipschitzContinuityAnalysis<ParameterType>> lipschitzContinuityAnalysis,
          const std::shared_ptr<LinearModelAnalysis<ParameterType>> linearModelAnalysis,
          const std::shared_ptr<QuadraticModelAnalysis<ParameterType>> quadraticModelAnalysis,
          const std::shared_ptr<AdditiveSeparabilityAnalysis<ParameterType>> additiveSeparabilityAnalysis) noexcept;

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
      std::vector<arma::Col<arma::uword>> getSeparations(
          const std::size_t& propertiesSetIndex) const;

      double getPlausibility(
          const std::size_t& propertiesSetIndex) const;

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
      std::vector<std::vector<arma::Col<arma::uword>>> separations_;

//      std::vector<bool> isProportionalSeparable_;
//      std::vector<std::unordered_map<arma::Col<arma::uword>, std::pair<arma::Col<arma::uword>, double>, Hasher, KeyEqual>> proportionalPartitions_;

      std::vector<double> plausibilities_;

      virtual void estimateImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept = 0;
  };

  //! Only FORWARD DECLERARTION of FULLY TEMPLATE SPECIALISATION from here on.
  //! Note: Forward declaration is needed to avoid ordering errors within the source file.

  // Nothing to see here, move along ...

  //! Only PARTIAL TEMPLATE SPECIALISATION from here on.
  //!
  //! Only PUBLIC methods from here on
  //! Note: Runtime checks are only performed for public methods.

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

  //! ALL METHODS SHOULD BE EITHER PROTECTED OR PRIVATE FROM HERE ON
  //! Note: Runtime checks are only performed for public methods.

  // Nothing to see here, move along ...
}
