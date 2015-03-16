namespace mant {
  class StandardParticleSwarmOptimisation2011 : public PopulationBasedOptimisationAlgorithm<double> {
    public:
      inline explicit StandardParticleSwarmOptimisation2011(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
          const unsigned int populationSize) noexcept;

      inline void setNeighbourhoodProbability(
          const double neighbourhoodProbability) noexcept;

      inline void setMaximalAcceleration(
          const double maximalAcceleration) noexcept;
      inline void setMaximalLocalAttraction(
          const double maximalLocalAttraction) noexcept;
      inline void setMaximalGlobalAttraction(
          const double maximalGlobalAttraction) noexcept;

      inline void setMaximalSwarmConvergence(
          const double maximalSwarmConvergence) noexcept;

      inline std::string toString() const noexcept override;

    protected:
      double neighbourhoodProbability_;

      double maximalAcceleration_;
      double maximalLocalAttraction_;
      double maximalGlobalAttraction_;

      double maximalSwarmConvergence_;

      arma::Mat<double> particles_;
      arma::Mat<double> velocities_;

      unsigned int particleIndex_;
      arma::Col<double> particle_;

      arma::Col<unsigned int> neighbourhoodParticlesIndecies_;
      unsigned int neighbourhoodBestParticleIndex_;

      arma::Col<double> attractionCenter_;

      arma::Mat<double> localBestSolutions_;
      arma::Row<double> localBestSoftConstraintsValues_;
      arma::Row<double> localBestObjectiveValues_;


      bool randomizeTopology_;

      arma::Mat<unsigned int> topology_;

      inline void optimiseImplementation() noexcept override;

      inline void initialiseSwarm() noexcept;

      inline arma::Mat<unsigned int> getRandomNeighbourhoodTopology() noexcept;

      inline double getAcceleration() noexcept;
      inline arma::Col<double> getVelocity() noexcept;
  };

  //
  // Implementation
  //

  inline StandardParticleSwarmOptimisation2011::StandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<double>(optimisationProblem, populationSize),
      localBestObjectiveValues_(this->populationSize_),
      randomizeTopology_(true) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(this->populationSize_), 3.0));
    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
    setMaximalSwarmConvergence(0.05); // TODO Check value within the paper
  }

  inline void StandardParticleSwarmOptimisation2011::optimiseImplementation() noexcept {
    initialiseSwarm();

    while(!this->isFinished() && !this->isTerminated()) {
      if (randomizeTopology_) {
        topology_ = getRandomNeighbourhoodTopology();
        randomizeTopology_ = false;
      }

      const arma::Col<unsigned int>& permutation = getRandomPermutation(this->populationSize_);
      for (std::size_t n = 0; n < this->populationSize_; ++n) {
        ++this->numberOfIterations_;

        particleIndex_ = permutation(n);
        particle_ = particles_.col(particleIndex_);

        neighbourhoodParticlesIndecies_ = arma::find(topology_.col(particleIndex_));
        static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies_)).min(neighbourhoodBestParticleIndex_);
        neighbourhoodBestParticleIndex_ = neighbourhoodParticlesIndecies_(neighbourhoodBestParticleIndex_);

        if (neighbourhoodBestParticleIndex_ == particleIndex_) {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_)) / 2.0;
        } else {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex_) - particle_)) / 3.0;
        }

        arma::Col<double> velocityCandidate = maximalAcceleration_ * getAcceleration() *  velocities_.col(particleIndex_) + getVelocity() * arma::norm(attractionCenter_) + attractionCenter_;
        arma::Col<double> solutionCandidate = particle_ + velocityCandidate;

        const arma::Col<unsigned int>& belowLowerBound = arma::find(solutionCandidate < this->optimisationProblem_->getLowerBounds());
        const arma::Col<unsigned int>& aboveUpperBound = arma::find(solutionCandidate > this->optimisationProblem_->getUpperBounds());

        velocityCandidate.elem(belowLowerBound) *= -0.5;
        velocityCandidate.elem(aboveUpperBound) *= -0.5;

        solutionCandidate.elem(belowLowerBound) = this->optimisationProblem_->getLowerBounds().elem(belowLowerBound);
        solutionCandidate.elem(aboveUpperBound) = this->optimisationProblem_->getUpperBounds().elem(aboveUpperBound);

        velocities_.col(particleIndex_) = velocityCandidate;
        particles_.col(particleIndex_) = solutionCandidate;

        const double& objectiveValue = this->optimisationProblem_->getObjectiveValue(solutionCandidate) + this->optimisationProblem_->getSoftConstraintsValue(solutionCandidate);

        if (objectiveValue < localBestObjectiveValues_(particleIndex_)) {
          localBestObjectiveValues_(particleIndex_) = objectiveValue;
          localBestSolutions_.col(particleIndex_) = solutionCandidate;
        }

        if (objectiveValue < this->bestObjectiveValue_) {
          this->bestObjectiveValue_ = objectiveValue;
          this->bestParameter_ = solutionCandidate;
        } else {
          randomizeTopology_ = true;
        }

        if (this->isFinished() || this->isTerminated()) {
          break;
        }
      }

      if (static_cast<arma::Row<double>>(arma::stddev(particles_, 1)).max() < maximalSwarmConvergence_) {
        initialiseSwarm();
      }
    }
  }

  inline void StandardParticleSwarmOptimisation2011::initialiseSwarm() noexcept {
    particles_ = arma::randu<arma::Mat<double>>(this->optimisationProblem_->numberOfDimensions_, this->populationSize_);
    particles_.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    particles_.each_col() += this->optimisationProblem_->getLowerBounds();

    velocities_ = arma::randu<arma::Mat<double>>(this->optimisationProblem_->numberOfDimensions_, this->populationSize_);
    velocities_.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    velocities_.each_col() += this->optimisationProblem_->getLowerBounds();
    velocities_ -= particles_;

    localBestSolutions_ = particles_;

    for (std::size_t n = 0; n < this->populationSize_; ++n) {
      ++this->numberOfIterations_;

      arma::Col<double> localBestSolution = localBestSolutions_.col(n);
      double localBestObjectiveValue = this->optimisationProblem_->getObjectiveValue(localBestSolution) + this->optimisationProblem_->getSoftConstraintsValue(localBestSolution);
      localBestObjectiveValues_(n) = localBestObjectiveValue;

      if (localBestObjectiveValue < this->bestObjectiveValue_) {
        this->bestParameter_ = localBestSolution;
        this->bestObjectiveValue_ = localBestObjectiveValue;
      }

      if (this->isFinished() || this->isTerminated()) {
        break;
      }
    }

    randomizeTopology_ = true;
  }

  inline double StandardParticleSwarmOptimisation2011::getAcceleration() noexcept {
    return std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  inline arma::Col<double> StandardParticleSwarmOptimisation2011::getVelocity() noexcept {
    return arma::normalise(arma::randn<arma::Col<double>>(this->optimisationProblem_->numberOfDimensions_)) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  inline arma::Mat<unsigned int> StandardParticleSwarmOptimisation2011::getRandomNeighbourhoodTopology() noexcept {
    arma::Mat<unsigned int> topology = (arma::randu<arma::Mat<double>>(this->populationSize_, this->populationSize_) <= neighbourhoodProbability_);
    topology.diag() += 1.0;

    return topology;
  }

  inline void StandardParticleSwarmOptimisation2011::setNeighbourhoodProbability(
      const double neighbourhoodProbability) noexcept {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  inline void StandardParticleSwarmOptimisation2011::setMaximalAcceleration(
      const double maximalAcceleration) noexcept {
    maximalAcceleration_ = maximalAcceleration;
  }

  inline void StandardParticleSwarmOptimisation2011::setMaximalLocalAttraction(
      const double maximalLocalAttraction) noexcept {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  inline void StandardParticleSwarmOptimisation2011::setMaximalGlobalAttraction(
      const double maximalGlobalAttraction) noexcept {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  inline void StandardParticleSwarmOptimisation2011::setMaximalSwarmConvergence(
      const double maximalSwarmConvergence) noexcept {
    maximalSwarmConvergence_ = maximalSwarmConvergence;
  }

  inline std::string StandardParticleSwarmOptimisation2011::toString() const noexcept {
    return "StandardParticleSwarmOptimisation2011";
  }
}
