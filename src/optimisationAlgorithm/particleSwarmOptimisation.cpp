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
        maximalGlobalAttraction_(arma::datum::nan) {
    setNextParametersFunction(
        [this](
            const arma::uword numberOfDimensions,
            const arma::Mat<double>& parameters,
            const arma::Col<double>& objectiveValues,
            const arma::Col<double>& differences) {
          if (differences(0) >= 0) {
            randomiseTopology_ = true;
          }
          
          if (activeParticleIndex_ == 0 && randomiseTopology_) {
            neighbourhoodTopology_ = neighbourhoodTopologyFunction_(numberOfParticles_);
            randomiseTopology_ = false;
          }
          
          if (objectiveValues(0) < localBestObjectiveValues_(activeParticleIndex_)) {
            localBestObjectiveValues_(activeParticleIndex_) = objectiveValues(0);
            localBestSolutions_.col(activeParticleIndex_) = parameters.col(0);
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
          maximalAcceleration_ * arma::randu<arma::Col<double>>(parameters.n_rows) % velocities_.col(activeParticleIndex_) + randomNeighbour(attractionCenter, 0, arma::norm(attractionCenter));

          particles_.col(activeParticleIndex_) += velocity;
          velocities_.col(activeParticleIndex_) = velocity;
            
          return particles_.col(activeParticleIndex_);
        },
        "Particle swarm optimisation");

    setBoundariesHandlingFunction([this](
        const arma::Mat<double>& parameters) {
      arma::Mat<double> boundedParameters = parameters;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<arma::uword>& belowLowerBound = arma::find(parameters.col(n) < 0);
        const arma::Col<arma::uword>& aboveUpperBound = arma::find(parameters.col(n) > 1);

        static_cast<arma::Col<double>>(velocities_.col(n)).elem(belowLowerBound) *= -0.5;
        static_cast<arma::Col<double>>(velocities_.col(n)).elem(aboveUpperBound) *= -0.5;

        static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(belowLowerBound).fill(0);
        static_cast<arma::Col<double>>(boundedParameters.col(n)).elem(aboveUpperBound).fill(1);
      }
      
      return boundedParameters;
    });

    setNeighbourhoodTopologyFunction([this](
        const arma::uword numberOfParticles) {
       arma::Mat<arma::uword> neighbourhoodTopology = (arma::randu<arma::Mat<double>>(numberOfParticles, numberOfParticles) <= std::pow(1.0 - 1.0 / static_cast<double>(numberOfParticles), 3.0));
       neighbourhoodTopology.diag().ones();
       
       return neighbourhoodTopology;
    });
  }

  void ParticleSwarmOptimisation::initialise(
      const arma::uword numberOfDimensions,
      const arma::Mat<double>& initialParameters) {
    if (!std::isfinite(maximalAcceleration_)) {
      setMaximalAcceleration(1.0 / (2.0 * std::log(2.0)));
    }

    if (!std::isfinite(maximalLocalAttraction_)) {
      setMaximalLocalAttraction(0.5 + std::log(2.0));
    }

    if (!std::isfinite(maximalGlobalAttraction_)) {
      setMaximalGlobalAttraction(maximalLocalAttraction_);
    }

    numberOfParticles_ = initialParameters.n_cols;
    activeParticleIndex_ = 0;
    particles_ = initialParameters;

    velocities_ = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParticles_) * 2 - 1;
    velocities_ -= initialParameters;

    localBestSolutions_ = initialParameters;
    localBestObjectiveValues_.set_size(numberOfParticles_);
    localBestObjectiveValues_.fill(arma::datum::inf);

    randomiseTopology_ = false;
    neighbourhoodTopology_ = neighbourhoodTopologyFunction_(numberOfParticles_);
  }

  void ParticleSwarmOptimisation::optimise(
      OptimisationProblem& optimisationProblem,
      const arma::uword numberOfParticles) {
    OptimisationAlgorithm::optimise(optimisationProblem, arma::randu<arma::Mat<double>>(optimisationProblem.numberOfDimensions_, numberOfParticles));
  }

  void ParticleSwarmOptimisation::setNeighbourhoodTopologyFunction(
      std::function<arma::Mat<arma::uword>(const arma::uword numberOfParticles)> neighbourhoodTopologyFunction) {
    verify(static_cast<bool>(neighbourhoodTopologyFunction), "setNeighbourhoodTopologyFunction: The neighbourhood topology function must be callable.");

    neighbourhoodTopologyFunction_ = neighbourhoodTopologyFunction;
  }

  void ParticleSwarmOptimisation::setMaximalAcceleration(
      const double maximalAcceleration) {
    verify(maximalAcceleration > 0, "setMaximalAcceleration: The maximal acceleration must be strict greater than 0.");

    maximalAcceleration_ = maximalAcceleration;
  }

  void ParticleSwarmOptimisation::setMaximalLocalAttraction(
      const double maximalLocalAttraction) {
    verify(maximalLocalAttraction > 0, "setMaximalLocalAttraction: The maximal local attraction must be strict greater than 0.");

    maximalLocalAttraction_ = maximalLocalAttraction;
  }

  void ParticleSwarmOptimisation::setMaximalGlobalAttraction(
      const double maximalGlobalAttraction) {
    verify(maximalGlobalAttraction > 0, "setMaximalGlobalAttraction: The maximal global attraction must be strict greater than 0.");

    maximalGlobalAttraction_ = maximalGlobalAttraction;
  }
}
