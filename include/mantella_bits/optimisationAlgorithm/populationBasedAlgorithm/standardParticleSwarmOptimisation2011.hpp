#pragma once

// C++ Standard Library
#include <cmath>
#include <random>

// Mantella
#include <mantella_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>
#include <mantella_bits/helper/random.hpp>
#include <mantella_bits/helper/rng.hpp>

namespace mant {
  template<class DistanceFunction>
  class StandardParticleSwarmOptimisation2011 : public PopulationBasedAlgorithm<double, DistanceFunction> {
    public:
      explicit StandardParticleSwarmOptimisation2011(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
          const unsigned int& populationSize) ;

      StandardParticleSwarmOptimisation2011(const StandardParticleSwarmOptimisation2011&) = delete;
      StandardParticleSwarmOptimisation2011& operator=(const StandardParticleSwarmOptimisation2011&) = delete;

      void setNeighbourhoodProbability(
          const double& neighbourhoodProbability) ;

      void setMaximalAcceleration(
          const double& maximalAcceleration) ;
      void setMaximalLocalAttraction(
          const double& maximalLocalAttraction) ;
      void setMaximalGlobalAttraction(
          const double& maximalGlobalAttraction) ;

      void setMaximalSwarmConvergence(
          const double& maximalSwarmConvergence) ;

      std::string to_string() const  override;

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

      void optimiseImplementation()  override;

      void initialiseSwarm() ;

      arma::Mat<unsigned int> getNeighbourhoodTopology() ;

      double getAcceleration() ;
      arma::Col<double> getVelocity() ;
  };

  template<class DistanceFunction>
  StandardParticleSwarmOptimisation2011<DistanceFunction>::StandardParticleSwarmOptimisation2011(
      const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
      const unsigned int& populationSize) 
    : PopulationBasedAlgorithm<double, EuclideanDistance>(optimisationProblem, populationSize),
      localBestObjectiveValues_(this->populationSize_),
      randomizeTopology_(true) {
    setNeighbourhoodProbability(std::pow(1.0 - 1.0 / static_cast<double>(this->populationSize_), 3.0));
    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
    setMaximalSwarmConvergence(0.05); // TODO Check value within the paper
  }

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::optimiseImplementation()  {
    initialiseSwarm();

    while(!this->isFinished() && !this->isTerminated()) {
      if (randomizeTopology_) {
        topology_ = getNeighbourhoodTopology();
        randomizeTopology_ = false;
      }

      const arma::Col<unsigned int>& permutation = getRandomPermutation(this->populationSize_);
      for (std::size_t n = 0; n < this->populationSize_; ++n) {
        ++this->numberOfIterations_;

        particleIndex_ = permutation.at(n);
        particle_ = particles_.col(particleIndex_);

        neighbourhoodParticlesIndecies_ = arma::find(topology_.col(particleIndex_));
        static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies_)).min(neighbourhoodBestParticleIndex_);
        neighbourhoodBestParticleIndex_ = neighbourhoodParticlesIndecies_.at(neighbourhoodBestParticleIndex_);

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

        if (objectiveValue < localBestObjectiveValues_.at(particleIndex_)) {
          localBestObjectiveValues_.at(particleIndex_) = objectiveValue;
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

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::initialiseSwarm()  {
    particles_ = arma::randu<arma::Mat<double>>(this->optimisationProblem_->getNumberOfDimensions(), this->populationSize_);
    particles_.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    particles_.each_col() += this->optimisationProblem_->getLowerBounds();

    velocities_ = arma::randu<arma::Mat<double>>(this->optimisationProblem_->getNumberOfDimensions(), this->populationSize_);
    velocities_.each_col() %= this->optimisationProblem_->getUpperBounds() - this->optimisationProblem_->getLowerBounds();
    velocities_.each_col() += this->optimisationProblem_->getLowerBounds();
    velocities_ -= particles_;

    localBestSolutions_ = particles_;

    for (std::size_t n = 0; n < this->populationSize_; ++n) {
      ++this->numberOfIterations_;

      arma::Col<double> localBestSolution = localBestSolutions_.col(n);
      double localBestObjectiveValue = this->optimisationProblem_->getObjectiveValue(localBestSolution) + this->optimisationProblem_->getSoftConstraintsValue(localBestSolution);
      localBestObjectiveValues_.at(n) = localBestObjectiveValue;

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

  template<class DistanceFunction>
  double StandardParticleSwarmOptimisation2011<DistanceFunction>::getAcceleration()  {
    return std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  template<class DistanceFunction>
  arma::Col<double> StandardParticleSwarmOptimisation2011<DistanceFunction>::getVelocity()  {
    return arma::normalise(arma::randn<arma::Col<double>>(this->optimisationProblem_->getNumberOfDimensions())) * std::uniform_real_distribution<double>(0.0, 1.0)(Rng::getGenerator());
  }

  template<class DistanceFunction>
  arma::Mat<unsigned int> StandardParticleSwarmOptimisation2011<DistanceFunction>::getNeighbourhoodTopology()  {
    arma::Mat<unsigned int> topology = (arma::randu<arma::Mat<double>>(this->populationSize_, this->populationSize_) <= neighbourhoodProbability_);
    topology.diag() += 1.0;

    return topology;
  }

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::setNeighbourhoodProbability(
      const double& neighbourhoodProbability)  {
    neighbourhoodProbability_ = neighbourhoodProbability;
  }

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::setMaximalAcceleration(
      const double& maximalAcceleration)  {
    maximalAcceleration_ = maximalAcceleration;
  }

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::setMaximalLocalAttraction(
      const double& maximalLocalAttraction)  {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::setMaximalGlobalAttraction(
      const double& maximalGlobalAttraction)  {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  template<class DistanceFunction>
  void StandardParticleSwarmOptimisation2011<DistanceFunction>::setMaximalSwarmConvergence(
      const double& maximalSwarmConvergence)  {
    maximalSwarmConvergence_ = maximalSwarmConvergence;
  }

  template<class DistanceFunction>
  std::string StandardParticleSwarmOptimisation2011<DistanceFunction>::to_string() const  {
    return "StandardParticleSwarmOptimisation2011";
  }
}
