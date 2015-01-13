#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis/continuityAnalysis/lipschitzContinuityAnalysis.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class DirectLipschitzContinuityAnalysis : public LipschitzContinuityAnalysis<ParameterType, DistanceFunction> {
    public:
      using LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::LipschitzContinuityAnalysis;

    protected:
      void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept override;
      void analyseImplementation(
          const std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>>& parameterToObjectiveValueMappings) noexcept override;
      void analyseImplementation(
          const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept override;
  };

  template <>
  void DirectLipschitzContinuityAnalysis<double, EuclideanDistance>::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

  template <typename ParameterType, class DistanceFunction>
  void DirectLipschitzContinuityAnalysis<ParameterType, DistanceFunction>::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<ParameterType>> optimisationProblem) noexcept {
    LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::lipschitzConstant_ = 0.0;

    const std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>>& parameterToObjectiveValueMappings = optimisationProblem->getCachedObjectiveValues();

    for (auto n = parameterToObjectiveValueMappings.cbegin(); n != parameterToObjectiveValueMappings.cend();) {
      const arma::Col<ParameterType>& parameter = n->first;
      const double& objectiveValue = n->second;
      for (auto k = ++n; k != parameterToObjectiveValueMappings.cend(); ++k) {
        LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::lipschitzConstant_ = std::max(LipschitzContinuityAnalysis<ParameterType, DistanceFunction>::lipschitzConstant_, std::abs(k->second - objectiveValue) / DistanceFunction::template getDistance<ParameterType>(k->first, parameter));
      }
    }
  }

  template <typename ParameterType, class DistanceFunction>
  void DirectLipschitzContinuityAnalysis<ParameterType, DistanceFunction>::analyseImplementation(
      const std::unordered_map<arma::Col<ParameterType>, double, Hash<arma::Col<ParameterType>>, IsKeyEqual<arma::Col<ParameterType>>>& parameterToObjectiveValueMappings) noexcept {

  }

  template <typename ParameterType, class DistanceFunction>
  void DirectLipschitzContinuityAnalysis<ParameterType, DistanceFunction>::analyseImplementation(
      const std::pair<arma::Col<ParameterType>, double>& parameterToObjectiveValueMapping) noexcept {

  }
}
