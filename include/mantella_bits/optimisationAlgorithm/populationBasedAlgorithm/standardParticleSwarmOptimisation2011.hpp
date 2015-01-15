#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class StandardParticleSwarmOptimisation2011 : public PopulationBasedAlgorithm<double, EuclideanDistance> {
    public:
      explicit StandardParticleSwarmOptimisation2011(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      StandardParticleSwarmOptimisation2011(const StandardParticleSwarmOptimisation2011&) = delete;
      StandardParticleSwarmOptimisation2011& operator=(const StandardParticleSwarmOptimisation2011&) = delete;

      void setNeighbourhoodProbability(
          const double& neighbourhoodProbability) noexcept;

      void setMaximalAcceleration(
          const double& maximalAcceleration) noexcept;
      void setMaximalLocalAttraction(
          const double& maximalLocalAttraction) noexcept;
      void setMaximalGlobalAttraction(
          const double& maximalGlobalAttraction) noexcept;

      void setMaximalSwarmConvergence(
          const double& maximalSwarmConvergence) noexcept;

      std::string to_string() const noexcept override;

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

      void optimiseImplementation() noexcept override;

      void initialiseSwarm() noexcept;

      arma::Mat<unsigned int> getNeighbourhoodTopology() noexcept;

      double getAcceleration() noexcept;
      arma::Col<double> getVelocity() noexcept;
  };
}
