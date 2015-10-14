#include <mantella_bits/propertyAnalysis/activePropertyAnalysis/additiveSeparabilityAnalysis.hpp>

// C++ standard library
#include <set>

// Mantella
#include <mantella_bits/helper/setTheory.hpp>

namespace mant {
  AdditiveSeparabilityAnalysis::AdditiveSeparabilityAnalysis(
      std::shared_ptr<OptimisationProblem> optimisationProblem)
      : ActivePropertyAnalysis(optimisationProblem),
        deviation_(std::numeric_limits<double>::infinity()) {
    setMaximalNumberOfIterations(1000);
  }

  std::vector<arma::Col<arma::uword>> AdditiveSeparabilityAnalysis::getPartition() {
    return partition_;
  }

  double AdditiveSeparabilityAnalysis::getDeviation() {
    return deviation_;
  }

  void AdditiveSeparabilityAnalysis::analyseImplementation() {
    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> partitionCandidates = getTwoSetsPartitions(optimisationProblem_->numberOfDimensions_);

    arma::Row<double> deviations(partitionCandidates.size(), arma::fill::zeros);
    for (arma::uword n = 0; n < partitionCandidates.size(); ++n) {
      std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>> partitionCandidate = partitionCandidates.at(n);
      for (arma::uword k = 0; k < maximalNumberOfIterations_; ++k) {
        arma::Col<double> firstSubPartA = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> firstSubPartB = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> secondSubPartA = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);
        arma::Col<double> secondSubPartB = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);

        arma::Col<double> parameterAA(optimisationProblem_->numberOfDimensions_);
        parameterAA.elem(partitionCandidate.first) = firstSubPartA;
        parameterAA.elem(partitionCandidate.second) = secondSubPartA;

        arma::Col<double> parameterBB(optimisationProblem_->numberOfDimensions_);
        parameterBB.elem(partitionCandidate.first) = firstSubPartB;
        parameterBB.elem(partitionCandidate.second) = secondSubPartB;

        arma::Col<double> parameterAB(optimisationProblem_->numberOfDimensions_);
        parameterAB.elem(partitionCandidate.first) = firstSubPartA;
        parameterAB.elem(partitionCandidate.second) = secondSubPartB;

        arma::Col<double> parameterBA(optimisationProblem_->numberOfDimensions_);
        parameterBA.elem(partitionCandidate.first) = firstSubPartB;
        parameterBA.elem(partitionCandidate.second) = secondSubPartA;

        deviations(n) += std::pow(
            (optimisationProblem_->getObjectiveValue(parameterAA) + optimisationProblem_->getObjectiveValue(parameterBB)) -
                (optimisationProblem_->getObjectiveValue(parameterAB) + optimisationProblem_->getObjectiveValue(parameterBA)),
            2.0);

        if (deviation_ + arma::datum::eps < deviations(n)) {
          break;
        }
      }
      deviation_ = std::min(deviation_, deviations(n));
    }

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> bestPartitionCandidates;
    for (const auto bestPartitionCandidateIndex : static_cast<arma::Row<arma::uword>>(arma::find(deviations <= deviation_ + arma::datum::eps))) {
      bestPartitionCandidates.push_back(partitionCandidates.at(bestPartitionCandidateIndex));
    }

    if (bestPartitionCandidates.size() > 1) {
      std::set<arma::uword> skipableDimensions;
      for (arma::uword n = 0; n < optimisationProblem_->numberOfDimensions_; ++n) {
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
          for (arma::uword k = n + 1; k < optimisationProblem_->numberOfDimensions_; ++k) {
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

          partition_.push_back(arma::Col<arma::uword>(part));
        }
      }
    } else {
      partition_ = {bestPartitionCandidates.at(0).first, bestPartitionCandidates.at(0).second};
    }
  }

  std::string AdditiveSeparabilityAnalysis::toString() const {
    return "additive_separability_analysis";
  }
}
