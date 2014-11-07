#pragma once

// C++ Standard Library
#include <memory>
#include <vector>

// Armadillo
#include <armadillo>

// HOP
#include <hop_bits/propertiesAnalysis.hpp>
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class PropertiesEstimation {
    public:
      explicit PropertiesEstimation();

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      PropertiesEstimation(const PropertiesEstimation&) = delete;
      PropertiesEstimation& operator=(const PropertiesEstimation&) = delete;

      virtual void estimate(
          const std::shared_ptr<OptimisationProblem>& optimisationProblem) const = 0;

      void addPropertiesAnalysis(
          const std::shared_ptr<PropertiesAnalysis> propertiesAnalysis);

      std::size_t getNumberOfPropertySets() const;

      bool isLinear(
          const std::size_t& propertiesSetIndex) const;
      arma::Col<double> getLinearEstimator(
          const std::size_t& propertiesSetIndex) const;

      bool isQuadratic(
          const std::size_t& propertiesSetIndex) const;
      arma::Col<double> getQuadraticEstimator(
          const std::size_t& propertiesSetIndex) const;

      bool isLipschitzContinuous(
          const std::size_t& propertiesSetIndex) const;
      double getLipschitzConstant(
          const std::size_t& propertiesSetIndex) const;

      bool isAlphaHoelderContinuous(
          const std::size_t& propertiesSetIndex) const;
      double getAlphaHoelderConstant(
          const std::size_t& propertiesSetIndex) const;

      double getBelief(
          const std::size_t& propertiesSetIndex) const;
      double getPlausibility(
          const std::size_t& propertiesSetIndex) const;
    protected:
      std::vector<std::shared_ptr<PropertiesAnalysis>> propertiesAnalyses_;

      std::size_t numberOfPropertySets_;

      std::vector<bool> isLinear_;
      std::vector<arma::Col<double>> linearEstimator_;

      std::vector<bool> isQuadratic_;
      std::vector<arma::Col<double>> quadraticEstimator_;

      std::vector<bool> isLipschitzContinuous_;
      std::vector<double> lipschitzConstant_;

      std::vector<bool> isAlphaHoelderContinuous_;
      std::vector<double> alphaHoelderConstant_;
  };
}
