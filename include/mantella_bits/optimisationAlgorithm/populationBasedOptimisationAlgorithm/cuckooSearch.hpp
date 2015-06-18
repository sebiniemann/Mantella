namespace mant {
  template <typename T>
  class CuckooSearch : public PopulationBasedOptimisationAlgorithm<T>{
    public:
      inline explicit CuckooSearch(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const unsigned int populationSize) noexcept;
		  
      inline void setDiscoveryProbability(
					const T prob) noexcept;
      inline void setLevyStepSize(
					const T stepSize) noexcept;
	  
			inline std::string toString() const noexcept override;

    protected:
      T discoveryProbability_;
      T levyStepSize_;

      inline void optimiseImplementation() override;
  };

  //
  //Implementation
  //

  template <typename T>
  inline CuckooSearch<T>::CuckooSearch(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize){
    setDiscoveryProbability(0.25);
    setLevyStepSize(1.0);
  }

  template <typename T>
  inline void CuckooSearch<T>::optimiseImplementation(){
    arma::Mat<T> hostNests = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, this->populationSize_);
    hostNests.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    hostNests.each_col() += this->getLowerBounds();

    arma::Col<T> nestFitness(this->populationSize_);
    for(std::size_t i = 0; i < this->populationSize_; ++i){
      nestFitness(i) = this->getObjectiveValue(hostNests.col(i));
			++this->numberOfIterations_;
    }
		unsigned int rankIndex;
		nestFitness.min(rankIndex);
		this->bestObjectiveValue_ = nestFitness(rankIndex);
		this->bestParameter_ = hostNests.col(rankIndex);

    while(!this->isFinished() && !this->isTerminated()){
			++this->numberOfIterations_;
      
			nestFitness.min(rankIndex);			
			arma::Col<T> newCuckoo = hostNests.col(rankIndex);
			
			//bei BentCigar gitb das in einer Expression bessere Ergebnisse aus
			//bei SphereFunction das aufgeteilte
			//Levy flights by Mantegna's algorithm
			arma::Col<T> u = arma::randn<arma::Col<T>>(this->numberOfDimensions_) * pow((tgamma(1+(3/2))*sin(M_PI*(3/2)/2)/(tgamma((1+(3/2))/2)*(3/2)*pow(2,(((3/2)-1)/2)))),(1/(3/2)));
			arma::Col<T> step = u/pow(abs(arma::randn<arma::Col<T>>(this->numberOfDimensions_)),1/(3/2));
			arma::Col<T> stepSize = levyStepSize_ * 0.01 * step % (newCuckoo - hostNests(rankIndex));
			newCuckoo += stepSize ;
			//in one expression:
			//newCuckoo += levyStepSize_ * 0.01 * ((arma::randn<arma::Col<T>>(this->numberOfDimensions_) 
			//						 * pow((tgamma(1+(3/2))*sin(M_PI*(3/2)/2)/(tgamma((1+(3/2))/2)*(3/2)*pow(2,(((3/2)-1)/2)))),(1/(3/2))))
			//						 / pow(abs(arma::randn<arma::Col<T>>(this->numberOfDimensions_)),1/(3/2))) % (newCuckoo - hostNests(rankIndex));
			
			const arma::Col<unsigned int>& belowLowerBound = arma::find(newCuckoo < this->getLowerBounds());
			const arma::Col<unsigned int>& aboveUpperBound = arma::find(newCuckoo > this->getUpperBounds());

			newCuckoo.elem(belowLowerBound) = this->getLowerBounds().elem(belowLowerBound);
			newCuckoo.elem(aboveUpperBound) = this->getUpperBounds().elem(aboveUpperBound);

			nestFitness.max(rankIndex);
			if(this->getObjectiveValue(newCuckoo) < nestFitness(rankIndex)){
				hostNests.col(rankIndex) = newCuckoo;
				nestFitness(rankIndex) = this->getObjectiveValue(hostNests.col(rankIndex));
			}			

			for(std::size_t i = 0; i < discoveryProbability_ * this->populationSize_; ++i){
				++this->numberOfIterations_;
					
				nestFitness.max(rankIndex);
				hostNests.col(rankIndex) = this->getRandomParameter();
				nestFitness(rankIndex) = this->getObjectiveValue(hostNests.col(rankIndex));
				if(this->isFinished() || this->isTerminated())break;
			}

			nestFitness.min(rankIndex);
			if (this->getObjectiveValue(hostNests.col(rankIndex)) < this->bestObjectiveValue_) {
				this->bestObjectiveValue_ = nestFitness(rankIndex);
				this->bestParameter_ = hostNests.col(rankIndex);
			}
			//funktioniert nicht
			//this->updateBestParameter(hostNests.col(rankIndex), 0.0, nestFitness(rankIndex));
    }
  }

  template <typename T>
  inline void CuckooSearch<T>::setDiscoveryProbability(
      const T prob) noexcept{
    discoveryProbability_ = prob;
  }

  template <typename T>
  inline void CuckooSearch<T>::setLevyStepSize(
      const T stepSize) noexcept{
    levyStepSize_ = stepSize;
  }

  template <typename T>
  inline std::string CuckooSearch<T>::toString() const noexcept {
    return "CuckooSearch";
  }
}
