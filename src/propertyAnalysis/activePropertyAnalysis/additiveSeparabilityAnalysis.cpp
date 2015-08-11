#include <mantella_bits/propertyAnalysis/activePropertyAnalysis/additiveSeparabilityAnalysis.hpp>

// C++ standard library
#include <unordered_set>

// Mantella
#include <mantella_bits/helper/setTheory.hpp>

namespace mant {
  AdditiveSeparabilityAnalysis::AdditiveSeparabilityAnalysis(
      std::shared_ptr<OptimisationProblem> optimisationProblem)
    : ActivePropertyAnalysis(optimisationProblem) {
      setMaximalNumberOfIterations(1000);
  }

  void AdditiveSeparabilityAnalysis::setMaximalNumberOfIterations(
        const arma::uword maximalNumberOfIterations) {
      maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  void AdditiveSeparabilityAnalysis::analyseImplementation() {
    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> partitionCandidates = getTwoSetsPartitions(optimisationProblem_->numberOfDimensions_);

    std::vector<std::vector<arma::Col<arma::uword>>> partitions;
    for (const auto& partitionCandidate : partitionCandidates) {
      arma::Col<double> differences(maximalNumberOfIterations_);

      for(arma::uword n = 0; n < differences.n_elem; ++n) {
        arma::Col<double> firstPartA = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> firstPartB = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> secondPartA = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);
        arma::Col<double> secondPartB = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);

        arma::Col<double> candidateA(optimisationProblem_->numberOfDimensions_, arma::fill::zeros);
        candidateA.elem(partitionCandidate.first) = firstPartA;
        candidateA.elem(partitionCandidate.second) = secondPartA;

        arma::Col<double> candidateB(optimisationProblem_->numberOfDimensions_, arma::fill::zeros);
        candidateB.elem(partitionCandidate.first) = firstPartA;
        candidateB.elem(partitionCandidate.second) = secondPartB;

        arma::Col<double> candidateC(optimisationProblem_->numberOfDimensions_, arma::fill::zeros);
        candidateC.elem(partitionCandidate.first) = firstPartB;
        candidateC.elem(partitionCandidate.second) = secondPartA;

        arma::Col<double> candidateD(optimisationProblem_->numberOfDimensions_, arma::fill::zeros);
        candidateD.elem(partitionCandidate.first) = firstPartB;
        candidateD.elem(partitionCandidate.second) = secondPartB;

        differences(n) =
            optimisationProblem_->getObjectiveValue(candidateA) +
            optimisationProblem_->getObjectiveValue(candidateD) -
            optimisationProblem_->getObjectiveValue(candidateB) -
            optimisationProblem_->getObjectiveValue(candidateC);
      }

//      if (arma::median(differences) < additiveSeparabilityMedianErrorThreshold_) {
        partitions.push_back({partitionCandidate.first, partitionCandidate.second});
//      }
    }

    std::unordered_set<arma::uword> singlePartitions;
    for (const auto& partition : partitions) {
      for (const auto& dimensions : partition) {
        if (dimensions.n_elem == 1) {
          singlePartitions.insert(arma::as_scalar(dimensions));
        }
      }
    }



//    std::map<std::pair<arma::uword, arma::uword>, arma::uword> pairedDimensions;
//    for(const auto& partition : partitions) {
//      for(const auto& dimensions : partition) {
//        if (dimensions.n_elem == 1) {
//          continue;
//        }

//        bool skip = false;
//        for(const auto& dimension : dimensions) {
//          if(singlePartitions.find(dimension) != singlePartitions.cend()) {
//            skip = true;
//            break;
//          }
//        }

//        if(skip) {
//          continue;
//        }

//        for(arma::uword n = 0; n < dimensions.n_elem; ++n) {
//          std::pair<arma::uword, arma::uword> pairedDimension = {n, (n + 1) % dimensions.n_elem};

//          const auto& cachePosition = pairedDimensions.find(pairedDimension);
//          if (cachePosition == pairedDimensions.cend()) {
//            pairedDimensions.insert({pairedDimension, 1});
//          } else {
//            ++cachePosition->second;
//          }
//        }
//      }
//    }
  }
  
  std::string AdditiveSeparabilityAnalysis::toString() const {
    return "additive_separability_analysis";
  }
}
