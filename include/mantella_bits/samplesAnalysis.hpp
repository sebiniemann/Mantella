#pragma once

// C++ standard library
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/armadillo.hpp"
// IWYU pragma: no_forward_declare mant::Hash
// IWYU pragma: no_forward_declare mant::IsEqual
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  double correlationAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<double(const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues)> correlationFunction);

  double fitnessDistanceCorrelation(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues);

  double continuityAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<double(const std::pair<arma::Col<double>, double>& firstSample, const std::pair<arma::Col<double>, double>& secondSample)> continuityFunction);

  double lipschitzContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample);

  double alphaHoelderContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample,
      const double alpha);

  arma::Col<double> polynomialFunctionModelAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword polynomialOrder,
      const std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Row<double>& objectiveValues)> estimatorFunction);

  std::vector<arma::Col<arma::uword>> separabilityAnalysis(
      OptimisationProblem& optimisationProblem,
      const std::function<double(const double firstOperand, const double secondOperand)> operatorFunction);

  double addition(
      const double firstOperand,
      const double secondOperand);

  double multiplication(
      const double firstOperand,
      const double secondOperand);

  std::vector<arma::Col<arma::uword>> proportionalityAnalysis(
      OptimisationProblem& optimisationProblem,
      const std::function<double(const double firstOperand, const double secondOperand)> proportionalityFunction);

  std::vector<arma::Col<arma::uword>> periodicityAnalysis(
      OptimisationProblem& optimisationProblem,
      const std::function<double(const double firstOperand, const double secondOperand)> periodicityFunction);
}
