namespace mant {
  template <typename T>
  class CuckooSearch : public PopulationBasedOptimisationAlgorithm<T> {
    public:
      explicit CuckooSearch(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const unsigned int populationSize) noexcept;
		  
      void setDiscoveryProbability(
					const double discoveryProbability) noexcept;
      void setLevyStepSize(
					const T levyStepSize) noexcept;
	  
			std::string toString() const noexcept override;

    protected:
      double discoveryProbability_;
      T levyStepSize_;

      void optimiseImplementation() override;
  };

  //
  //Implementation
  //

  template <typename T>
  CuckooSearch<T>::CuckooSearch(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    setDiscoveryProbability(0.25);
    setLevyStepSize(1.0);
  }

  template <typename T>
  void CuckooSearch<T>::optimiseImplementation() {
    arma::Mat<T> nests = this->getRandomPopulation();

    arma::Col<double> objectiveValues(this->populationSize_);
    for (std::size_t n = 0; n < this->populationSize_; ++n) {
			++this->numberOfIterations_;
      this->updateBestParameter(nests.col(n), this->getSoftConstraintsValue(nests.col(n)), this->getObjectiveValue(nests.col(n)));
    }
    
		unsigned int rankIndex;
    while(!this->isFinished() && !this->isTerminated()){
			++this->numberOfIterations_;
      
			objectiveValues.min(rankIndex);			
      const arma::Col<T>& cuckooCandidate = this->boundaryHandling(nests.col(rankIndex) + levyStepSize_ * 0.01 * ((arma::randn<arma::Col<T>>(this->numberOfDimensions_) * std::pow(std::tgamma(2.5) * std::sin(arma::datum::pi * 0.75) / (std::tgamma(1.25) * 1.5 * std::pow(2, 0.25)), 2/3)) / arma::pow(arma::abs(arma::randn<arma::Col<T>>(this->numberOfDimensions_)), 2/3)) % (nests.col(rankIndex) - this->bestParameter_));
      const double objectiveValue = this->getObjectiveValue(cuckooCandidate);
      
			objectiveValues.max(rankIndex);
			if(objectiveValue < objectiveValues(rankIndex)) {
				nests.col(rankIndex) = cuckooCandidate;
				objectiveValues(rankIndex) = objectiveValue;
			}			
      
			this->updateBestParameter(cuckooCandidate, 0.0, objectiveValue);
      
      if(this->isFinished() || this->isTerminated()) {
        break;
      }
        
			for(std::size_t i = 0; i < std::floor(discoveryProbability_ * this->populationSize_); ++i){
				++this->numberOfIterations_;
					
				objectiveValues.max(rankIndex);
				nests.col(rankIndex) = this->getRandomParameter();
				objectiveValues(rankIndex) = this->getObjectiveValue(nests.col(rankIndex));
        
        this->updateBestParameter(nests.col(rankIndex), 0.0, objectiveValues(rankIndex));
      
				if(this->isFinished() || this->isTerminated()) {
          break;
        }
			}
    }
  }

  template <typename T>
  void CuckooSearch<T>::setDiscoveryProbability(
      const double discoveryProbability) noexcept {
    discoveryProbability_ = discoveryProbability;
  }

  template <typename T>
  void CuckooSearch<T>::setLevyStepSize(
      const T levyStepSize) noexcept {
    levyStepSize_ = levyStepSize;
  }

  template <typename T>
  std::string CuckooSearch<T>::toString() const noexcept {
    return "cuckoo_search";
  }
}
