namespace mant {
  class ParallelStandardParticleSwarmOptimisation2011 : public ParallelAlgorithm<double> {
    public:
      inline explicit ParallelStandardParticleSwarmOptimisation2011(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
          const unsigned int populationSize) noexcept;

      inline void setNeighbourhoodProbability(
          const double neighbourhoodProbability) noexcept;
      inline void setAcceleration(
          const double acceleration) noexcept;
      inline void setLocalAttraction(
          const double localAttraction) noexcept;
      inline void setGlobalAttraction(
          const double globalAttraction) noexcept;
      inline void setCommunicationSteps(
          const unsigned int communicationSteps) noexcept;

      inline std::string toString() const noexcept override;

    protected:
      double neighbourhoodProbability_;
      double acceleration_;
      double localAttraction_;
      double globalAttraction_;

      unsigned int communicationSteps_;

      inline void parallelOptimiseImplementation() noexcept override;
  };

  //
  // Implementation
  //

  ParallelStandardParticleSwarmOptimisation2011::ParallelStandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : ParallelAlgorithm<double>(optimisationProblem, populationSize) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(this->populationSize_), 3.0));
    setAcceleration(1.0 / (2.0 * std::log(2.0)));
    setLocalAttraction(0.5 + std::log(2.0));
    setGlobalAttraction(localAttraction_);
    setCommunicationSteps(1);
  }

  void ParallelStandardParticleSwarmOptimisation2011::parallelOptimiseImplementation() noexcept {
    arma::Mat<double> localParticles = arma::randu<arma::Mat<double>>(this->optimisationProblem_->numberOfDimensions_, this->populationSize_);
    localParticles.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    localParticles.each_col() += this->optimisationProblem_->getLowerBounds();

    arma::Mat<double> localVelocities = arma::randu<arma::Mat<double>>(this->optimisationProblem_->numberOfDimensions_, this->populationSize_);
    localVelocities.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    localVelocities.each_col() += this->optimisationProblem_->getLowerBounds();
    localVelocities -= localParticles;

    arma::Mat<double> localBestSolutions(this->optimisationProblem_->numberOfDimensions_, this->populationSize_ * this->numberOfNodes_);
    localBestSolutions.cols(this->rank_ * this->populationSize_, (this->rank_ + 1) * this->populationSize_ - 1) = localParticles;

    arma::Col<double> localBestObjectiveValues(this->populationSize_ * this->numberOfNodes_);
    for (std::size_t n = 0; n < this->populationSize_; ++n) {
      ++this->numberOfIterations_;
      arma::Col<double> localBestSolution = localBestSolutions.col(this->rank_ * this->populationSize_ + n);
      double localBestObjectiveValue = this->optimisationProblem_->getObjectiveValue(localBestSolution) + this->optimisationProblem_->getSoftConstraintsValue(localBestSolution);
      localBestObjectiveValues(this->rank_ * this->populationSize_ + n) = localBestObjectiveValue;

      if (this->isFinished() || this->isTerminated()) {
        break;
      }
    }

    arma::Mat<double> localBestSolutionsSend = localBestSolutions.cols(this->rank_ * this->populationSize_, (this->rank_ + 1) * this->populationSize_ - 1);
    MPI_Allgather(
          localBestSolutionsSend.memptr(),
          this->populationSize_ * this->optimisationProblem_->numberOfDimensions_,
          MPI_DOUBLE,
          localBestSolutions.memptr(),
          this->populationSize_ * this->optimisationProblem_->numberOfDimensions_,
          MPI_DOUBLE,
          MPI_COMM_WORLD);

    arma::Col<double> localBestObjectiveValuesSend = localBestObjectiveValues.subvec(this->rank_ * this->populationSize_, (this->rank_ + 1) * this->populationSize_ - 1);
    MPI_Allgather(
          localBestObjectiveValuesSend.memptr(),
          this->populationSize_,
          MPI_DOUBLE,
          localBestObjectiveValues.memptr(),
          this->populationSize_,
          MPI_DOUBLE,
          MPI_COMM_WORLD);

    unsigned int bestSolutionIndex;
    this->bestObjectiveValue_ = localBestObjectiveValues.min(bestSolutionIndex);
    this->bestParameter_ = localBestSolutions.col(bestSolutionIndex);

    bool randomizeTopology = true;

    arma::Mat<unsigned int> topology(this->populationSize_ * this->numberOfNodes_, this->populationSize_);
    while(!this->isFinished() && !this->isTerminated()) {
      for (unsigned int k = 0; k < communicationSteps_; ++k) {
        if (randomizeTopology) {
            topology = (arma::randu<arma::Mat<double>>(this->populationSize_ * this->numberOfNodes_, this->populationSize_) <= neighbourhoodProbability_);
            topology.diag(-this->rank_ * this->populationSize_) += 1.0;

            randomizeTopology = false;
        }

        arma::Col<unsigned int> permutation = getRandomPermutation(this->populationSize_);
        for (std::size_t n = 0; n < this->populationSize_; ++n) {
          ++this->numberOfIterations_;

          std::size_t k = permutation(n);
          arma::Col<double> particle = localParticles.col(k);

          unsigned int neighbourhoodBestParticleIndex;
          arma::Col<unsigned int> neighbourhoodParticlesIndecies = arma::find(topology.col(k));
          static_cast<arma::Col<double>>(localBestObjectiveValues.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

          neighbourhoodBestParticleIndex = neighbourhoodParticlesIndecies(neighbourhoodBestParticleIndex);

          arma::Col<double> attractionCenter;
          if (neighbourhoodBestParticleIndex == this->rank_ * this->populationSize_ + k) {
            attractionCenter = (localAttraction_ * (localBestSolutions.col(this->rank_ * this->populationSize_ + k) - particle)) / 2.0;
          } else {
            attractionCenter = (localAttraction_ * (localBestSolutions.col(this->rank_ * this->populationSize_ + k) - particle) + globalAttraction_ * (localBestSolutions.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
          }

          arma::Col<double> velocityCandidate = acceleration_ * localVelocities.col(k) + arma::normalise(arma::randn<arma::Col<double>>(this->optimisationProblem_->numberOfDimensions_)) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator()) * arma::norm(attractionCenter) + attractionCenter;
          arma::Col<double> solutionCandidate = particle + velocityCandidate;

          arma::Col<unsigned int> belowLowerBound = arma::find(solutionCandidate < this->optimisationProblem_->getLowerBounds());
          arma::Col<unsigned int> aboveUpperBound = arma::find(solutionCandidate > this->optimisationProblem_->getUpperBounds());

          velocityCandidate.elem(belowLowerBound) *= -0.5;
          velocityCandidate.elem(aboveUpperBound) *= -0.5;

          solutionCandidate.elem(belowLowerBound) = this->optimisationProblem_->getLowerBounds().elem(belowLowerBound);
          solutionCandidate.elem(aboveUpperBound) = this->optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

          localVelocities.col(k) = velocityCandidate;
          localParticles.col(k) = solutionCandidate;

          double objectiveValue = this->optimisationProblem_->getObjectiveValue(solutionCandidate) + this->optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

          if (objectiveValue < localBestObjectiveValues(k)) {
            localBestObjectiveValues(this->rank_ * this->populationSize_ + k) = objectiveValue;
            localBestSolutions.col(this->rank_ * this->populationSize_ + k) = solutionCandidate;
          }

          if (objectiveValue < this->bestObjectiveValue_) {
            this->bestObjectiveValue_ = objectiveValue;
            this->bestParameter_ = solutionCandidate;
          } else {
            randomizeTopology = true;
          }

          if (this->isFinished() || this->isTerminated()) {
            break;
          }
        }

        if (this->isFinished() || this->isTerminated()) {
          break;
        }
      }

      arma::Mat<double> localBestSolutionsSend = localBestSolutions.cols(this->rank_ * this->populationSize_, (this->rank_ + 1) * this->populationSize_ - 1);
      MPI_Allgather(
            localBestSolutionsSend.memptr(),
            this->populationSize_ * this->optimisationProblem_->numberOfDimensions_,
            MPI_DOUBLE,
            localBestSolutions.memptr(),
            this->populationSize_ * this->optimisationProblem_->numberOfDimensions_,
            MPI_DOUBLE,
            MPI_COMM_WORLD);

      arma::Col<double> localBestObjectiveValuesSend = localBestObjectiveValues.subvec(this->rank_ * this->populationSize_, (this->rank_ + 1) * this->populationSize_ - 1);
      MPI_Allgather(
            localBestObjectiveValuesSend.memptr(),
            this->populationSize_,
            MPI_DOUBLE,
            localBestObjectiveValues.memptr(),
            this->populationSize_,
            MPI_DOUBLE,
            MPI_COMM_WORLD);

      unsigned int bestSolutionIndex;
      double bestObjectiveValue = localBestObjectiveValues.min(bestSolutionIndex);
      if (bestObjectiveValue < this->bestObjectiveValue_) {
        this->bestObjectiveValue_ = bestObjectiveValue;
        this->bestParameter_ = localBestSolutions.col(bestSolutionIndex);
      } else {
        randomizeTopology = true;
      }
    }
  }

  inline void ParallelStandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(
      const double neighbourhoodProbability) noexcept {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  inline void ParallelStandardParticleSwarmOptimisation2011::setAcceleration(
      const double acceleration) noexcept {
    acceleration_ = acceleration;
  }

  inline void ParallelStandardParticleSwarmOptimisation2011::setLocalAttraction(
      const double localAttraction) noexcept {
    localAttraction_ = localAttraction;
  }

  inline void ParallelStandardParticleSwarmOptimisation2011::setGlobalAttraction(
      const double globalAttraction) noexcept {
    globalAttraction_ = globalAttraction;
  }

  inline void ParallelStandardParticleSwarmOptimisation2011::setCommunicationSteps(
      const unsigned int communicationSteps) noexcept {
    communicationSteps_ = communicationSteps;
  }

  inline std::string ParallelStandardParticleSwarmOptimisation2011::toString() const noexcept {
    return "ParallelStandardParticleSwarmOptimisation2011";
  }
}
