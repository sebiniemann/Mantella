#pragma once

// C++ Standard Library
#include <memory>
#include <vector>

// Armadillo
#include <armadillo>

// HOP
#include <hop_bits/optimisationProblem.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/correlationAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/separabilityAnalysis/additiveSeparabilityAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/activePropertiesAnalysis/proportionalityAnalysis/linearProportionalityAnalysis.hpp>

namespace hop {
  class PropertiesEstimation {
    public:
      explicit PropertiesEstimation(
          const std::shared_ptr<CorrelationAnalysis> correlationAnalysis,
          const std::shared_ptr<LinearModelAnalysis> linearModelAnalysis,
          const std::shared_ptr<QuadraticModelAnalysis> quadraticModelAnalysis,
          const std::shared_ptr<LipschitzContinuityAnalysis> lipschitzContinuityAnalysis,
          const std::shared_ptr<AdditiveSeparabilityAnalysis> additiveSeparabilityAnalysis) noexcept;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesEstimation(const PropertiesEstimation&) = delete;
      PropertiesEstimation& operator=(const PropertiesEstimation&) = delete;

      void estimate(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept;

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
      std::vector<arma::Col<arma::uword>> getPartitions(
          const std::size_t& propertiesSetIndex) const;

      double getPlausibility(
          const std::size_t& propertiesSetIndex) const;

    protected:
      std::shared_ptr<CorrelationAnalysis> correlationAnalysis_;
      std::shared_ptr<LinearModelAnalysis> linearModelAnalysis_;
      std::shared_ptr<QuadraticModelAnalysis> quadraticModelAnalysis_;
      std::shared_ptr<LipschitzContinuityAnalysis> lipschitzContinuityAnalysis_;
      std::shared_ptr<AdditiveSeparabilityAnalysis> additiveSeparabilityAnalysis_;

      std::size_t numberOfPropertySets_;

      std::vector<bool> isLinear_;
      std::vector<arma::Col<double>> linearModelEstimators_;

      std::vector<bool> isQuadratic_;
      std::vector<arma::Col<double>> quadraticModelEstimators_;

      std::vector<double> lipschitzConstants_;

      std::vector<double> correlationCoefficient_;

      std::vector<bool> isSeparable_;
      std::vector<std::vector<arma::Col<arma::uword>>> partitions_;

//      std::vector<bool> isProportionalSeparable_;
//      std::vector<std::unordered_map<arma::Col<arma::uword>, std::pair<arma::Col<arma::uword>, double>, Hasher, KeyEqual>> proportionalPartitions_;

      virtual void estimateImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) noexcept = 0;
  };
}
