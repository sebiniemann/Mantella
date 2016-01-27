#include "mantella_bits/optimisationAlgorithm/particleSwarmOptimisation.hpp"

// C++ standard library
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"
#include "mantella_bits/optimisationProblem.hpp"
#include "mantella_bits/probability.hpp"

namespace mant {
  ParticleSwarmOptimisation::ParticleSwarmOptimisation()
      : OptimisationAlgorithm(),
        maximalAcceleration_(arma::datum::nan),
        maximalLocalAttraction_(arma::datum::nan),
        maximalGlobalAttraction_(arma::datum::nan),
        numberOfParticles_(0),
        activeParticleIndex_(0),
        randomiseTopology_(false) {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions_,
            const arma::Mat<double>& parameters_,
            const arma::Row<double>& objectiveValues_,
            const arma::Row<double>& differences_) {
          if (differences_(0) >= 0) {
            randomiseTopology_ = true;
          }
          
          if (activeParticleIndex_ == 0 && randomiseTopology_) {
            neighbourhoodTopology_ = neighbourhoodTopologyFunction_();
            randomiseTopology_ = false;
          }
          
          if (objectiveValues_(0) < localBestObjectiveValues_(activeParticleIndex_)) {
            localBestObjectiveValues_(activeParticleIndex_) = objectiveValues_(0);
            localBestSolutions_.col(activeParticleIndex_) = parameters_.col(0);
          }
          
          activeParticleIndex_ = (activeParticleIndex_ + 1) % numberOfParticles_;
          const arma::Col<double>& particle = particles_.col(activeParticleIndex_);
          
          arma::uword neighbourhoodBestParticleIndex;
          arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(neighbourhoodTopology_.col(activeParticleIndex_));
          static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

          arma::Col<double> attractionCenter;
          if (neighbourhoodParticlesIndecies(neighbourhoodBestParticleIndex) == activeParticleIndex_) {
            attractionCenter = (maximalLocalAttraction_ * (localBestSolutions_.col(activeParticleIndex_) - particle)) / 2.0;
          } else {
            attractionCenter = (maximalLocalAttraction_ * (localBestSolutions_.col(activeParticleIndex_) - particle) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
          }

          const arma::Col<double>& velocity =
          maximalAcceleration_ * arma::randu<arma::Col<double>>(numberOfDimensions_) % velocities_.col(activeParticleIndex_) + randomNeighbour(attractionCenter, 0, arma::norm(attractionCenter));

          particles_.col(activeParticleIndex_) += velocity;
          velocities_.col(activeParticleIndex_) = velocity;
            
          return particles_.col(activeParticleIndex_);
        },
        "Particle swarm optimisation");

    setBoundariesHandlingFunction(
        [this](
            const arma::Mat<double>& parameters_,
            const arma::Mat<arma::uword>& isBelowLowerBound_,
            const arma::Mat<arma::uword>& isAboveUpperBound_) {
          arma::Mat<double> boundedParameters = parameters_;
          
          velocities_.elem(arma::find(isBelowLowerBound_)) *= -0.5;
          velocities_.elem(arma::find(isAboveUpperBound_)) *= -0.5;
          
          boundedParameters.elem(arma::find(isBelowLowerBound_)).fill(0);
          boundedParameters.elem(arma::find(isAboveUpperBound_)).fill(1);
          
          return boundedParameters;
        });

    setNeighbourhoodTopologyFunction(
        [this]() {
           arma::Mat<arma::uword> neighbourhoodTopology = (arma::randu<arma::Mat<double>>(numberOfParticles_, numberOfParticles_) <= std::pow(1.0 - 1.0 / static_cast<double>(numberOfParticles_), 3.0));
           neighbourhoodTopology.diag().ones();
           
           return neighbourhoodTopology;
        },
        "Random");

    setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    setMaximalLocalAttraction(0.5 + std::log(2.0));
    setMaximalGlobalAttraction(maximalLocalAttraction_);
  }

  void ParticleSwarmOptimisation::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    numberOfParticles_ = initialParameters.n_cols;
    activeParticleIndex_ = 0;
    particles_ = initialParameters;

    velocities_ = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParticles_) * 2 - 1;
    velocities_ -= initialParameters;

    localBestSolutions_ = initialParameters;
    localBestObjectiveValues_.set_size(numberOfParticles_);
    localBestObjectiveValues_.fill(arma::datum::inf);

    randomiseTopology_ = false;
    neighbourhoodTopology_ = neighbourhoodTopologyFunction_();
  }

  void ParticleSwarmOptimisation::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfParticles) {
    optimise(optimisationProblem, arma::randu<arma::Mat<double>>(optimisationProblem.numberOfDimensions_, numberOfParticles));
  }

  void ParticleSwarmOptimisation::optimise(
      OptimisationProblem& optimisationProblem) {
    optimise(optimisationProblem, 40);
  }

  void ParticleSwarmOptimisation::setNeighbourhoodTopologyFunction(
      std::function<arma::Mat<arma::uword>()> neighbourhoodTopologyFunction,
      const std::string& neighbourhoodTopologyFunctionName) {
    verify(static_cast<bool>(neighbourhoodTopologyFunction), "setNeighbourhoodTopologyFunction: The neighbourhood topology function must be callable.");

    neighbourhoodTopologyFunction_ = neighbourhoodTopologyFunction;
    neighbourhoodTopologyFunctionName_ = neighbourhoodTopologyFunctionName;
  }

  void ParticleSwarmOptimisation::setNeighbourhoodTopologyFunction(
      std::function<arma::Mat<arma::uword>()> neighbourhoodTopologyFunction) {
    setNeighbourhoodTopologyFunction(neighbourhoodTopologyFunction, "Unnamed, custom neighbourhood topology function");
  }

  std::string ParticleSwarmOptimisation::getNeighbourhoodTopologyFunctionName() const {
    return neighbourhoodTopologyFunctionName_;
  }

  void ParticleSwarmOptimisation::setMaximalAcceleration(
      const double maximalAcceleration) {
    maximalAcceleration_ = maximalAcceleration;
  }

  double ParticleSwarmOptimisation::getMaximalAcceleration() const {
    return maximalAcceleration_;
  }

  void ParticleSwarmOptimisation::setMaximalLocalAttraction(
      const double maximalLocalAttraction) {
    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  double ParticleSwarmOptimisation::getMaximalLocalAttraction() const {
    return maximalLocalAttraction_;
  }

  void ParticleSwarmOptimisation::setMaximalGlobalAttraction(
      const double maximalGlobalAttraction) {
    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }

  double ParticleSwarmOptimisation::getMaximalGlobalAttraction() const {
    return maximalGlobalAttraction_;
  }
}
