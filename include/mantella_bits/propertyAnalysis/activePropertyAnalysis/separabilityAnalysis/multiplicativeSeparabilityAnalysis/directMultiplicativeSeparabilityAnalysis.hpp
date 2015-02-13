namespace mant {
  class DirectMultiplicativeSeparabilityAnalysis : public MultiplicativeSeparabilityAnalysis<double> {
    public:
      using MultiplicativeSeparabilityAnalysis<double>::MultiplicativeSeparabilityAnalysis;

    protected:
      inline void analyseImplementation(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept override;
  };

  //
  // Implementation
  //

  inline void DirectMultiplicativeSeparabilityAnalysis::analyseImplementation(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept {
    std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> partitionCandidates = getTwoSetsPartitions(optimisationProblem->getNumberOfDimensions());

    std::vector<std::vector<arma::Col<unsigned int>>> partitions;
    for (const auto& partitionCandidate : partitionCandidates) {
      arma::Col<double> differences(maximalNumberOfIterations_);

      for(std::size_t n = 0; n < differences.n_elem; ++n) {
        arma::Col<double> firstPartA = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> firstPartB = arma::randu<arma::Col<double>>(partitionCandidate.first.n_elem);
        arma::Col<double> secondPartA = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);
        arma::Col<double> secondPartB = arma::randu<arma::Col<double>>(partitionCandidate.second.n_elem);

        arma::Col<double> candidateA(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateA.elem(partitionCandidate.first) = firstPartA;
        candidateA.elem(partitionCandidate.second) = secondPartA;

        arma::Col<double> candidateB(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateB.elem(partitionCandidate.first) = firstPartA;
        candidateB.elem(partitionCandidate.second) = secondPartB;

        arma::Col<double> candidateC(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateC.elem(partitionCandidate.first) = firstPartB;
        candidateC.elem(partitionCandidate.second) = secondPartA;

        arma::Col<double> candidateD(optimisationProblem->getNumberOfDimensions(), arma::fill::zeros);
        candidateD.elem(partitionCandidate.first) = firstPartB;
        candidateD.elem(partitionCandidate.second) = secondPartB;

        differences.at(n) =
            optimisationProblem->getObjectiveValue(candidateA) *
            optimisationProblem->getObjectiveValue(candidateD) /
            optimisationProblem->getObjectiveValue(candidateB) /
            optimisationProblem->getObjectiveValue(candidateC);
      }

      if (arma::median(differences) < additiveSeparabilityMedianErrorThreshold_) {
        partitions.push_back({partitionCandidate.first, partitionCandidate.second});
      }
    }

    std::unordered_set<unsigned int> singlePartitions;
    for (const auto& partition : partitions) {
      for (const auto& dimensions : partition) {
        if (dimensions.n_elem == 1) {
          singlePartitions.insert(arma::as_scalar(dimensions));
        }
      }
    }



//    std::map<std::pair<unsigned int, unsigned int>, unsigned int> pairedDimensions;
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

//        for(std::size_t n = 0; n < dimensions.n_elem; ++n) {
//          std::pair<unsigned int, unsigned int> pairedDimension = {n, (n + 1) % dimensions.n_elem};

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
}
