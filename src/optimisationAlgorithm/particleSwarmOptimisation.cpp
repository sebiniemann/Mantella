#include "mantella_bits/optimisationAlgorithm/particleSwarmOptimisation.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

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
          if (arma::any(differences_ >= 0)) {
            randomiseTopology_ = true;
          }

          if (randomiseTopology_) {
            neighbourhoodTopology_ = neighbourhoodTopologyFunction_();
            randomiseTopology_ = false;
          }

          for (arma::uword n = 0; n < particles_.n_cols; ++n) {
            if (objectiveValues_(n) < localBestObjectiveValues_(n)) {
              localBestObjectiveValues_(n) = objectiveValues_(n);
              localBestSolutions_.col(n) = parameters_.col(n);
            }
          }

          for (arma::uword n = 0; n < particles_.n_cols; ++n) {

            const arma::Col<double>& particle = particles_.col(n);

            arma::uword neighbourhoodBestParticleIndex;
            arma::Col<arma::uword> neighbourhoodParticlesIndecies = arma::find(neighbourhoodTopology_.col(n));
            static_cast<arma::Col<double>>(localBestObjectiveValues_.elem(neighbourhoodParticlesIndecies)).min(neighbourhoodBestParticleIndex);

            arma::Col<double> attractionCenter;
            if (neighbourhoodParticlesIndecies(neighbourhoodBestParticleIndex) == n) {
              attractionCenter = (maximalLocalAttraction_ * (localBestSolutions_.col(n) - particle)) / 2.0;
            } else {
              attractionCenter = (maximalLocalAttraction_ * (localBestSolutions_.col(n) - particle) + maximalGlobalAttraction_ * (localBestSolutions_.col(neighbourhoodBestParticleIndex) - particle)) / 3.0;
            }

            const arma::Col<double>& velocity = maximalAcceleration_ * arma::randu<arma::Col<double>>(numberOfDimensions_) % velocities_.col(n) + randomNeighbour(attractionCenter, 0, arma::norm(attractionCenter));

            particles_.col(n) += velocity;
            velocities_.col(n) = velocity;
          }

          return particles_;
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

#if defined(SUPPORT_MPI)
    setCommunicationFunction(
        [this]() {
          arma::Col<double> localDataTable(numberOfDimensions_ + 1);
          arma::Mat<double> worldDataTable(numberOfDimensions_ + 1, numberOfNodes_);

          localDataTable(0) = bestObjectiveValue_;
          localDataTable.tail_rows(numberOfDimensions_) = bestParameter_;

          if(MPI_Allgather(
              localDataTable.memptr(),
              numberOfDimensions_ + 1,
              MPI_DOUBLE,
              worldDataTable.memptr(),
              numberOfDimensions_ + 1,
              MPI_DOUBLE,
              MPI_COMM_WORLD) != MPI_SUCCESS)
          {
            std::cout << " ParticleSwarmOptimisation: MPI_Allgather failed." << std::endl;
          }

          arma::uword bestSolutionIndex;
          bestObjectiveValue_ = worldDataTable.row(0).min(bestSolutionIndex);
          bestParameter_ = worldDataTable(arma::span(1, numberOfDimensions_), bestSolutionIndex);
        },
        "MPI");
#endif
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
    verify(static_cast<bool>(neighbourhoodTopologyFunction), "ParticleSwarmOptimisation.setNeighbourhoodTopologyFunction: The neighbourhood-topology function must be callable.");

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
