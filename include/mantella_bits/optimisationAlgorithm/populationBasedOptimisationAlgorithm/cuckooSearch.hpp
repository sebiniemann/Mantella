namespace mant{
  class CuckooSearch : public PopulationBasedOptimisationAlgorithm<double>{
    public:
      inline explicit CuckooSearch(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
          const unsigned int populationSize) noexcept;

      inline std::string toString() const noexcept override;

      inline void setDiscoveryProbability(double prob);
      inline void setLevyStepSize(double stepSize);

    protected:
      double discoveryProbability_;
      double levyStepSize_;

      inline void optimiseImplementation() override;
  };

  //
  //Implementation
  //

  inline CuckooSearch::CuckooSearch(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<double>(optimisationProblem, populationSize){
    setDiscoveryProbability(0.25);
    setLevyStepSize(1.0);
  }

  inline void CuckooSearch::optimiseImplementation(){
    arma::Mat<double> hostNests = arma::randu<arma::Mat<double>>(this->numberOfDimensions_, this->populationSize_);
    hostNests.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    hostNests.each_col() += this->getLowerBounds();

    arma::Col<double> nestFitness(this->populationSize_);
    for(std::size_t i = 0; i < this->populationSize_; ++i){
        nestFitness(i) = this->getObjectiveValue(hostNests.col(i));
    }

    while(!this->isFinished() && !this->isTerminated()){
      ++this->numberOfIterations_;

      //index of random selected nest
      unsigned int randPos = rand() % this->populationSize_;

      //current index of the bets/worst solution
      std::size_t rankIndex;
      nestFitness.min(rankIndex);

      arma::Col<double> newCuckoo = hostNests.col(randPos);

      //performing a Levy Flight
      arma::Col<double> u = arma::randn<arma::Col<double>>(this->numberOfDimensions_) * pow((tgamma(1+(3/2))*sin(M_PI*(3/2)/2)/(tgamma((1+(3/2))/2)*(3/2)*pow(2,(((3/2)-1)/2)))),(1/(3/2)));
      arma::Col<double> step = u/pow(abs(arma::randn<arma::Col<double>>(this->numberOfDimensions_)),1/(3/2));

      arma::Col<double> stepSize = levyStepSize_ * 0.01 * step % (newCuckoo- hostNests(rankIndex));

      newCuckoo += stepSize % arma::randn<arma::Col<double>>(this->numberOfDimensions_);

      //check if newCuckoo is inside Bounds
      const arma::Col<unsigned int>& belowLowerBound = arma::find(newCuckoo < this->getLowerBounds());
      const arma::Col<unsigned int>& aboveUpperBound = arma::find(newCuckoo > this->getUpperBounds());

      newCuckoo.elem(belowLowerBound) = this->getLowerBounds().elem(belowLowerBound);
      newCuckoo.elem(aboveUpperBound) = this->getUpperBounds().elem(aboveUpperBound);

      randPos = rand() % this->populationSize_;
      arma::Col<double> comparisonNest = hostNests.col(randPos);
      if(this->getObjectiveValue(newCuckoo) < this->getObjectiveValue(comparisonNest)){
        hostNests.col(randPos) = newCuckoo;
        nestFitness(randPos) = this->getObjectiveValue(hostNests.col(randPos));
      }



      //abondoning worse nests
      for(std::size_t i = 0; i < discoveryProbability_ * this->populationSize_; ++i){
        nestFitness.max(rankIndex);
        hostNests.col(rankIndex) = arma::Col<double>(this->populationSize_, arma::fill::randu);
        hostNests.col(rankIndex) %= this->getUpperBounds() - this->getLowerBounds();
        hostNests.col(rankIndex) += this->getLowerBounds();
        nestFitness(rankIndex) = this->getObjectiveValue(hostNests.col(rankIndex));
      }

      //finding the index of the best Solution
      nestFitness.min(rankIndex);
      if (this->getObjectiveValue(hostNests.col(rankIndex)) < this->bestObjectiveValue_) {
        this->bestObjectiveValue_ = nestFitness(rankIndex);
        this->bestParameter_ = hostNests.col(rankIndex);

        this->bestSoftConstraintsValue_ = 0.0;
      }
    }
  }

  inline void CuckooSearch::setDiscoveryProbability(
      double prob){
    discoveryProbability_ = prob;
  }

  inline void CuckooSearch::setLevyStepSize(
      double stepSize){
    levyStepSize_ = stepSize;
  }

  inline std::string CuckooSearch::toString() const noexcept {
    return "CuckooSearch";
  }
}
