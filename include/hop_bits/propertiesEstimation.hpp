#pragma once

// C++ Standard Library
#include <memory>
#include <vector>

// Armadillo
#include <armadillo>

// HOP
#include <hop_bits/optimisationProblem.hpp>
#include <hop_bits/propertiesAnalysis/functionModelAnalysis/linearModelAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/functionModelAnalysis/quadraticModelAnalysis.hpp>
#include <hop_bits/propertiesAnalysis/lipschitzContinuityAnalysis.hpp>

namespace hop {
  class PropertiesEstimation {
    public:
      explicit PropertiesEstimation(
          const std::shared_ptr<LinearModelAnalysis> linearModelAnalysis,
          const std::shared_ptr<QuadraticModelAnalysis> quadraticModelAnalysis,
          const std::shared_ptr<LipschitzContinuityAnalysis> lipschitzContinuityAnalysis);

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesEstimation(const PropertiesEstimation&) = delete;
      PropertiesEstimation& operator=(const PropertiesEstimation&) = delete;

      void estimate(
          const std::shared_ptr<OptimisationProblem> optimisationProblem);

      std::size_t getNumberOfPropertySets() const;

      bool isLinear(
          const std::size_t& propertiesSetIndex) const;
      arma::Col<double> getLinearModelEstimator(
          const std::size_t& propertiesSetIndex) const;

      bool isQuadratic(
          const std::size_t& propertiesSetIndex) const;
      arma::Col<double> getQuadraticModelEstimator(
          const std::size_t& propertiesSetIndex) const;

      bool isLipschitzContinuous(
          const std::size_t& propertiesSetIndex) const;
      double getLipschitzConstant(
          const std::size_t& propertiesSetIndex) const;

      bool isAlphaHoelderContinuous(
          const std::size_t& propertiesSetIndex) const;
      double getAlphaHoelderConstant(
          const std::size_t& propertiesSetIndex) const;

      double getPlausibility(
          const std::size_t& propertiesSetIndex) const;

    protected:
      std::shared_ptr<LinearModelAnalysis> linearModelAnalysis_;
      std::shared_ptr<QuadraticModelAnalysis> quadraticModelAnalysis_;
      std::shared_ptr<LipschitzContinuityAnalysis> lipschitzContinuityAnalysis_;

      std::size_t numberOfPropertySets_;

      std::vector<bool> isLinear_;
      std::vector<arma::Col<double>> linearModelEstimators_;

      std::vector<bool> isQuadratic_;
      std::vector<arma::Col<double>> quadraticModelEstimators_;

      std::vector<bool> isLipschitzContinuous_;
      std::vector<double> lipschitzConstants_;

      std::vector<bool> isAlphaHoelderContinuous_;
      std::vector<double> alphaHoelderConstants_;

      virtual void estimateImplementation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem) = 0;
  };
}
