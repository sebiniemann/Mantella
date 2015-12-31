#include "mantella_bits/samplesAnalysis.hpp"

// C++ standard library
#include <cmath>
#include <set>

// Mantella
#include "mantella_bits/algebra.hpp"
#include "mantella_bits/numberTheory.hpp"

namespace mant {
  double correlationAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<double(const arma::Mat<double>& parameters, const arma::Col<double>& objectiveValues)> correlationFunction) {
    verify(samples.size() > 1, "correlationAnalysis: The number of samples must be at least 2.");

    arma::Mat<double> parameters(samples.cbegin()->first.n_elem, samples.size());
    arma::Row<double> objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for (const auto& sample : samples) {
      parameters.col(n) = sample.first;
      objectiveValues(n) = sample.second;
      ++n;
    }

    return correlationFunction(parameters, objectiveValues);
  }

  double fitnessDistanceCorrelation(
      const arma::Mat<double>& parameters,
      const arma::Row<double>& objectiveValues) {
    arma::uword bestParameterIndex;
    objectiveValues.min(bestParameterIndex);

    arma::Mat<double> remainingParameters = parameters;
    remainingParameters.shed_col(bestParameterIndex);
    remainingParameters.each_col() -= parameters.col(bestParameterIndex);
    
    arma::Row<double> remainingObjectiveValues = objectiveValues;
    remainingObjectiveValues.shed_col(bestParameterIndex);

    return arma::as_scalar(arma::cor(arma::sqrt(arma::sum(arma::square(remainingParameters))), remainingObjectiveValues));
  }

  double continuityAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const std::function<double(const std::pair<arma::Col<double>, double>& firstSample, const std::pair<arma::Col<double>, double>& secondSample)> continuityFunction) {
    double continuity = 0.0;
    for (auto firstSample = samples.cbegin(); firstSample != samples.cend();) {
      for (auto secondSample = ++firstSample; secondSample != samples.cend(); ++secondSample) {
        continuity = std::max(continuity, continuityFunction({firstSample->first, firstSample->second}, {secondSample->first, secondSample->second}));
      }
    }
    
    return continuity;
  }

  double lipschitzContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample) {
    return std::abs(firstSample.second - secondSample.second) / arma::norm(firstSample.first - secondSample.first);
  }

  double alphaHoelderContinuity(
      const std::pair<arma::Col<double>, double>& firstSample,
      const std::pair<arma::Col<double>, double>& secondSample,
      const double alpha) {
    if (alpha < 1.0) {
      return std::abs(firstSample.second - secondSample.second) / std::pow(arma::norm(firstSample.first - secondSample.first), alpha);
    } else {
      return 1.0;
    }
  }

  arma::Col<double> polynomialFunctionModelAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples,
      const arma::uword polynomialOrder,
      const std::function<arma::Col<double>(const arma::Mat<double>& parameters, const arma::Row<double>& objectiveValues)> estimatorFunction) {
    verify(samples.size() > 1, "correlationAnalysis: The number of samples must be at least 2.");

    arma::Mat<double> parameters(polynomialSize(samples.cbegin()->first.n_elem, polynomialOrder), samples.size());
    arma::Row<double> objectiveValues(parameters.n_cols);

    arma::uword n = 0;
    for (const auto& sample : samples) {
      parameters.col(n) = polynomial(sample.first, polynomialOrder);
      objectiveValues(n) = sample.second;
      ++n;
    }

    return estimatorFunction(parameters, objectiveValues);
  }

  std::vector<arma::Col<arma::uword>> separabilityAnalysis(
      OptimisationProblem& optimisationProblem,
      const std::function<double(const double firstOperand, const double secondOperand)> operatorFunction) {
    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> partitionCandidates = twoSetsPartitions(optimisationProblem.numberOfDimensions_);

    double deviation = 1000.0;
    arma::Row<double> deviations(partitionCandidates.size(), arma::fill::zeros);
    for (arma::uword n = 0; n < partitionCandidates.size(); ++n) {
      std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>> partitionCandidate = partitionCandidates.at(n);
      for (arma::uword k = 0; k < 1000; ++k) {
        arma::Col<double> firstSubPartA = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> firstSubPartB = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> secondSubPartA = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);
        arma::Col<double> secondSubPartB = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);

        arma::Col<double> parameterAA(optimisationProblem.numberOfDimensions_);
        parameterAA.elem(partitionCandidate.first) = firstSubPartA;
        parameterAA.elem(partitionCandidate.second) = secondSubPartA;

        arma::Col<double> parameterBB(optimisationProblem.numberOfDimensions_);
        parameterBB.elem(partitionCandidate.first) = firstSubPartB;
        parameterBB.elem(partitionCandidate.second) = secondSubPartB;

        arma::Col<double> parameterAB(optimisationProblem.numberOfDimensions_);
        parameterAB.elem(partitionCandidate.first) = firstSubPartA;
        parameterAB.elem(partitionCandidate.second) = secondSubPartB;

        arma::Col<double> parameterBA(optimisationProblem.numberOfDimensions_);
        parameterBA.elem(partitionCandidate.first) = firstSubPartB;
        parameterBA.elem(partitionCandidate.second) = secondSubPartA;

        deviations(n) += std::pow(
            (operatorFunction(optimisationProblem.getObjectiveValue(parameterAA), optimisationProblem.getObjectiveValue(parameterBB))) -
                (operatorFunction(optimisationProblem.getObjectiveValue(parameterAB), optimisationProblem.getObjectiveValue(parameterBA))),
            2.0);

        if (deviation + arma::datum::eps < deviations(n)) {
          break;
        }
      }
      deviation = std::min(deviation, deviations(n));
    }

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> bestPartitionCandidates;
    for (const auto bestPartitionCandidateIndex : static_cast<arma::Row<arma::uword>>(arma::find(deviations <= deviation + arma::datum::eps))) {
      bestPartitionCandidates.push_back(partitionCandidates.at(bestPartitionCandidateIndex));
    }

    std::vector<arma::Col<arma::uword>> partition;
    if (bestPartitionCandidates.size() > 1) {
      std::set<arma::uword> skipableDimensions;
      for (arma::uword n = 0; n < optimisationProblem.numberOfDimensions_; ++n) {
        if (skipableDimensions.find(n) != skipableDimensions.cend()) {
          skipableDimensions.erase(n);
        } else {
          std::vector<arma::Col<arma::uword>> contatiningPartitions;
          for (const auto& bestPartitionCandidate : bestPartitionCandidates) {
            if (!static_cast<arma::Col<arma::uword>>(arma::find(bestPartitionCandidate.first == n)).is_empty()) {
              contatiningPartitions.push_back(bestPartitionCandidate.first);
            } else {
              contatiningPartitions.push_back(bestPartitionCandidate.second);
            }
          }

          std::vector<arma::uword> part;
          part.push_back(n);
          for (arma::uword k = n + 1; k < optimisationProblem.numberOfDimensions_; ++k) {
            bool isWithinAllPartitions = true;
            for (const auto& contatiningPartition : contatiningPartitions) {
              if (static_cast<arma::Col<arma::uword>>(arma::find(contatiningPartition == k)).is_empty()) {
                isWithinAllPartitions = false;
                break;
              }
            }

            if (isWithinAllPartitions) {
              part.push_back(k);
              skipableDimensions.insert(k);
            }
          }

          partition.push_back(arma::Col<arma::uword>(part));
        }
      }
    } else {
      partition = {bestPartitionCandidates.at(0).first, bestPartitionCandidates.at(0).second};
    }
    
    return partition;
  }

  double addition(
      const double firstOperand,
      const double secondOperand) {
    return firstOperand + secondOperand;
  }

  double multiplication(
      const double firstOperand,
      const double secondOperand) {
    return firstOperand * secondOperand;
  }

  std::vector<arma::Col<arma::uword>> proportionalityAnalysis(
      OptimisationProblem& optimisationProblem,
      const std::function<double(const double firstOperand, const double secondOperand)> proportionalityFunction) {
    std::vector<arma::Col<arma::uword>> result;

    return result;
  }

  std::vector<arma::Col<arma::uword>> periodicityAnalysis(
      OptimisationProblem& optimisationProblem,
      const std::function<double(const double firstOperand, const double secondOperand)> periodicityFunction) {
    std::vector<arma::Col<arma::uword>> result;

    return result;
  }
}
