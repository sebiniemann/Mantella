namespace mant {
  template <typename T = double>
  class AdditiveSeparabilityAnalysis : public ActivePropertyAnalysis<T> {
    static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
    
    public:
      explicit AdditiveSeparabilityAnalysis() noexcept;

      void setMaximalNumberOfIterations(
            const unsigned long long maximalNumberOfIterations) noexcept;

      std::string toString() const noexcept override;
            
    protected:
      unsigned long long maximalNumberOfIterations_;

      void analyseImplementation(
          std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept override;
  };

  //
  // Implementation
  //

  template <typename T>
  AdditiveSeparabilityAnalysis<T>::AdditiveSeparabilityAnalysis() noexcept {
      setMaximalNumberOfIterations(1000);
  }

  template <typename T>
  void AdditiveSeparabilityAnalysis<T>::setMaximalNumberOfIterations(
        const unsigned long long maximalNumberOfIterations) noexcept {
      maximalNumberOfIterations_ = maximalNumberOfIterations;
  }

  template <typename T>
  void AdditiveSeparabilityAnalysis<T>::analyseImplementation(
      std::shared_ptr<OptimisationProblem<T>> optimisationProblem) noexcept {
    std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> partitionCandidates = getTwoSetsPartitions(optimisationProblem->numberOfDimensions_);

    std::vector<std::vector<arma::Col<unsigned int>>> partitions;
    for (const auto& partitionCandidate : partitionCandidates) {
      arma::Col<double> differences(maximalNumberOfIterations_);

      for(std::size_t n = 0; n < differences.n_elem; ++n) {
        arma::Col<T> firstPartA = arma::randu<arma::Col<T>>(partitionCandidate.first.n_elem);
        arma::Col<T> firstPartB = arma::randu<arma::Col<T>>(partitionCandidate.first.n_elem);
        arma::Col<T> secondPartA = arma::randu<arma::Col<T>>(partitionCandidate.second.n_elem);
        arma::Col<T> secondPartB = arma::randu<arma::Col<T>>(partitionCandidate.second.n_elem);

        arma::Col<T> candidateA(optimisationProblem->numberOfDimensions_, arma::fill::zeros);
        candidateA.elem(partitionCandidate.first) = firstPartA;
        candidateA.elem(partitionCandidate.second) = secondPartA;

        arma::Col<T> candidateB(optimisationProblem->numberOfDimensions_, arma::fill::zeros);
        candidateB.elem(partitionCandidate.first) = firstPartA;
        candidateB.elem(partitionCandidate.second) = secondPartB;

        arma::Col<T> candidateC(optimisationProblem->numberOfDimensions_, arma::fill::zeros);
        candidateC.elem(partitionCandidate.first) = firstPartB;
        candidateC.elem(partitionCandidate.second) = secondPartA;

        arma::Col<T> candidateD(optimisationProblem->numberOfDimensions_, arma::fill::zeros);
        candidateD.elem(partitionCandidate.first) = firstPartB;
        candidateD.elem(partitionCandidate.second) = secondPartB;

        differences(n) =
            optimisationProblem->getObjectiveValue(candidateA) +
            optimisationProblem->getObjectiveValue(candidateD) -
            optimisationProblem->getObjectiveValue(candidateB) -
            optimisationProblem->getObjectiveValue(candidateC);
      }

//      if (arma::median(differences) < additiveSeparabilityMedianErrorThreshold_) {
        partitions.push_back({partitionCandidate.first, partitionCandidate.second});
//      }
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
  
  template <typename T>
  std::string AdditiveSeparabilityAnalysis<T>::toString() const noexcept {
    return "additive_separability_analysis";
  }
}
