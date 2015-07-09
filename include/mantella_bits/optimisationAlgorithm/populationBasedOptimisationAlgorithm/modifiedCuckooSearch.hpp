namespace mant {
  template <typename T>
  class ModifiedModifiedCuckooSearch : public PopulationBasedOptimisationAlgorithm<T> {
    public:
      explicit ModifiedModifiedCuckooSearch(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const unsigned int populationSize) noexcept;
		  
      void setTopNestPortion(
					const double topNestPortion) noexcept;
			void setWorstNestPortion(
					const double worstNestPortion) noexcept;
      void setMaxLevyStepSize(
					const double maxLevyStepSize) noexcept;
	  
			std::string toString() const noexcept override;

    protected:
      double topNestPortion_;
			double worstNestPortion_;
      double maxLevyStepSize_;

      void optimiseImplementation() override;
  };

  //
  //Implementation
  //

	template <typename T>
  ModifiedModifiedCuckooSearch<T>::ModifiedCuckooSearch(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize) {
    setTopNestPortion(0.25);
		setWorstNestPortion(0.75);
    setMaxLevyStepSize(1.0);
  }

  template <typename T>
  void ModifiedCuckooSearch<T>::optimiseImplementation() {
		const double goldenRatio = (1 + std::sqrt(5))/2;
    arma::Mat<T> nests = this->getRandomPopulation();

    arma::Col<double> objectiveValues(this->populationSize_);
    for (std::size_t n = 0; n < this->populationSize_; ++n) {
			++this->numberOfIterations_;
			objectiveValues(n) = this->getObjectiveValue(nests.col(n));
      this->updateBestParameter(nests.col(n), this->getSoftConstraintsValue(nests.col(n)), objectiveValues(n));
    }
    
		unsigned int rankIndex;
    while(!this->isFinished() && !this->isTerminated()) {
			++this->numberOfIterations_;
			arma::Col<unsigned int> indices = arma::sort_index(objectiveValues);
			
			for(std::size_t i = std::ceil((1 - worstNestPortion_) * this->populationSize_); i < this->populationSize_; ++i) {
				++this->numberOfIterations_;
				double levyStepSize = maxLevyStepSize_ / std::sqrt(this->numberOfIterations_);	
				
				nests.col(indices(i)) = levyFlight(indices(i), levyStepSize);
				objectiveValues(indices(i)) = this->getObjectiveValue(nests.col(n));
				this->updateBestParameter(nests.col(indices(i)), 0.0, objectiveValues(indices(i)));
			}
			
			for(std::size_t i = 0; i < std::floor(topNestPortion_ * this->populationSize_); ++i) {
				++this->numberOfIterations_;
				unsigned int randTopNest = getRandomTopNest();
				if(randTopNest = indices(i)) {
					double levyStepSize = maxLevyStepSize_ / std::pow(this->numberOfIterations_, 2);
					
					const arma::Col<T>& cuckooCandidate = levyFlight(indices(i), levyStepSize);
					const double objectiveValue = this->getObjectiveValue(cuckooCandidate);
					
					unsigned int randNest = getRandomNest();
					if(objectiveValue < objectiveValues(randNest)) {
						nests(randNest) = cuckooCandidate;
						objectiveValues(randNest) = objectiveValue;
						this->updateBestParameter(cuckooCandidate, 0.0, objectiveValue);
					}
				}
				else {
					//generating new Nest on the line between the two chosen Nests
					const arma::Col<T>& cuckooCandidate = arma::abs(nests(indices(i)) - nests(randTopNest)) / goldenRatio;
					const double objectiveValue = this->getObjectiveValue(cuckooCandidate);
					
					unsigned int randNest = getRandomNest();
					if(objectiveValue < objectiveValues(randNest)) {
						nests(randNest) = cuckooCandidate;
						objectiveValues(randNest) = objectiveValue;
						this->updateBestParameter(cuckooCandidate, 0.0, objectiveValue);
					}
				}
			}
    }
  }
	//TODO random funktionen und levyFlight funktion
	
	
  template <typename T>
  void ModifiedModifiedCuckooSearch<T>::setTopNestPortion(
			const double topNestPortion) noexcept {
    topNestPortion_ = topNestPortion;
  }
	
	template <typename T>
  void ModifiedModifiedCuckooSearch<T>::setWorstNestPortion(
			const double worstNestPortion) noexcept {
    worstNestPortion_ = worstNestPortion;
  }

  template <typename T>
  void ModifiedModifiedCuckooSearch<T>::setMaxLevyStepSize(
      const double MaxLevyStepSize) noexcept {
    maxLevyStepSize_ = maxLevyStepSize;
  }

  template <typename T>
  std::string ModifiedModifiedCuckooSearch<T>::toString() const noexcept {
    return "modified_cuckoo_search";
  }
}
