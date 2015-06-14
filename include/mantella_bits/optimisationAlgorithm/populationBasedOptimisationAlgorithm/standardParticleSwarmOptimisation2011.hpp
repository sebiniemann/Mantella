namespace mant {
  template <typename T = double>
  class StandardParticleSwarmOptimisation2011 : public PopulationBasedOptimisationAlgorithm<T> {
    public:
      inline explicit StandardParticleSwarmOptimisation2011(
          const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
          const unsigned int populationSize) noexcept;

      inline void setNeighbourhoodProbability(
          const T neighbourhoodProbability) noexcept;

      inline void setMaximalAcceleration(
          const T maximalAcceleration) noexcept;
      inline void setMaximalLocalAttraction(
          const T maximalLocalAttraction) noexcept;
      inline void setMaximalGlobalAttraction(
          const T maximalGlobalAttraction) noexcept;

      inline void setMaximalSwarmConvergence(
          const T maximalSwarmConvergence) noexcept;

      inline std::string toString() const noexcept override;

    protected:
      T neighbourhoodProbability_;

      T maximalAcceleration_;
      T maximalLocalAttraction_;
      T maximalGlobalAttraction_;

      T maximalSwarmConvergence_;

      arma::Mat<T> particles_;
      arma::Mat<T> velocities_;

      unsigned int particleIndex_;
      arma::Col<T> particle_;

      arma::Col<unsigned int> neighbourhoodParticlesIndecies_;
      unsigned int neighbourhoodBestParticleIndex_;

      arma::Col<T> attractionCenter_;

      arma::Mat<T> localBestSolutions_;
      arma::Row<double> localBestSoftConstraintsValues_;
      arma::Row<double> localBestObjectiveValues_;


      bool randomizeTopology_;

      arma::Mat<unsigned int> topology_;

      inline void optimiseImplementation() noexcept override;

      inline void initialiseSwarm() noexcept;

      inline arma::Mat<unsigned int> getRandomNeighbourhoodTopology() noexcept;

      inline T getAcceleration() noexcept;
      inline arma::Col<T> getVelocity() noexcept;
  };

  //
  // Implementation
  //

  template <typename T>
  inline StandardParticleSwarmOptimisation2011<T>::StandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem<T>> optimisationProblem,
      const unsigned int populationSize) noexcept
    : PopulationBasedOptimisationAlgorithm<T>(optimisationProblem, populationSize),
      localBestObjectiveValues_(this->populationSize_),
      randomizeTopology_(true) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<T>(this->populationSize_), 3.0));
    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
    setMaximalSwarmConvergence(0.05); // TODO Check value within the paper
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::optimiseImplementation() noexcept {
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
        static_cast<arma::Col<T>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies_)).min(neighbourhoodBestParticleIndex_);
        neighbourhoodBestParticleIndex_ = neighbourhoodParticlesIndecies_(neighbourhoodBestParticleIndex_);

        if (neighbourhoodBestParticleIndex_ == particleIndex_) {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_)) / 2.0;
        } else {
          attractionCenter_ = (maximalLocalAttraction_ * (localBestSolutions_.col(particleIndex_) - particle_) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex_) - particle_)) / 3.0;
        }

        arma::Col<T> velocityCandidate = maximalAcceleration_ * getAcceleration() *  velocities_.col(particleIndex_) + getVelocity() * arma::norm(attractionCenter_) + attractionCenter_;
        arma::Col<T> solutionCandidate = particle_ + velocityCandidate;

        const arma::Col<unsigned int>& belowLowerBound = arma::find(solutionCandidate < this->getLowerBounds());
        const arma::Col<unsigned int>& aboveUpperBound = arma::find(solutionCandidate > this->getUpperBounds());

        velocityCandidate.elem(belowLowerBound) *= -0.5;
        velocityCandidate.elem(aboveUpperBound) *= -0.5;

        solutionCandidate.elem(belowLowerBound) = this->getLowerBounds().elem(belowLowerBound);
        solutionCandidate.elem(aboveUpperBound) = this->getUpperBounds().elem(aboveUpperBound);

        velocities_.col(particleIndex_) = velocityCandidate;
        particles_.col(particleIndex_) = solutionCandidate;

        const double& objectiveValue = this->getObjectiveValue(solutionCandidate) + this->getSoftConstraintsValue(solutionCandidate);

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

      if (static_cast<arma::Row<T>>(arma::stddev(particles_, 1)).max() < maximalSwarmConvergence_) {
        initialiseSwarm();
      }
    }
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::initialiseSwarm() noexcept {
    particles_ = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, this->populationSize_);
    particles_.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    particles_.each_col() += this->getLowerBounds();

    velocities_ = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, this->populationSize_);
    velocities_.each_col() %= this->getUpperBounds() - this->getLowerBounds();
    velocities_.each_col() += this->getLowerBounds();
    velocities_ -= particles_;

    localBestSolutions_ = particles_;

    for (std::size_t n = 0; n < this->populationSize_; ++n) {
      ++this->numberOfIterations_;

      arma::Col<double> localBestSolution = localBestSolutions_.col(n);
      double localBestObjectiveValue = this->getObjectiveValue(localBestSolution) + this->getSoftConstraintsValue(localBestSolution);
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

  template <typename T>
  inline T StandardParticleSwarmOptimisation2011<T>::getAcceleration() noexcept {
    return std::uniform_real_distribution<T>(0.0, 1.0)(Rng::getGenerator());
  }

  template <typename T>
  inline arma::Col<T> StandardParticleSwarmOptimisation2011<T>::getVelocity() noexcept {
    return arma::normalise(arma::randn<arma::Col<T>>(this->numberOfDimensions_)) * std::uniform_real_distribution<T>(0.0, 1.0)(Rng::getGenerator());
  }

  template <typename T>
  inline arma::Mat<unsigned int> StandardParticleSwarmOptimisation2011<T>::getRandomNeighbourhoodTopology() noexcept {
    arma::Mat<unsigned int> topology = (arma::randu<arma::Mat<T>>(this->populationSize_, this->populationSize_) <= neighbourhoodProbability_);
    topology.diag() += 1.0;

    return topology;
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::setNeighbourhoodProbability(
      const T neighbourhoodProbability) noexcept {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::setMaximalAcceleration(
      const T maximalAcceleration) noexcept {
    maximalAcceleration_ = maximalAcceleration;
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::setMaximalLocalAttraction(
      const T maximalLocalAttraction) noexcept {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::setMaximalGlobalAttraction(
      const T maximalGlobalAttraction) noexcept {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  template <typename T>
  inline void StandardParticleSwarmOptimisation2011<T>::setMaximalSwarmConvergence(
      const T maximalSwarmConvergence) noexcept {
    maximalSwarmConvergence_ = maximalSwarmConvergence;
  }

  template <typename T>
  inline std::string StandardParticleSwarmOptimisation2011<T>::toString() const noexcept {
    return "StandardParticleSwarmOptimisation2011";
  }
}
