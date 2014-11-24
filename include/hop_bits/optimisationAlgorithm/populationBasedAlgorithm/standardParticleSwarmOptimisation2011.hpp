#pragma once

// HOP
#include <hop_bits/optimisationAlgorithm/populationBasedAlgorithm.hpp>

namespace hop {
  class StandardParticleSwarmOptimisation2011 : public PopulationBasedAlgorithm {
    public:
      explicit StandardParticleSwarmOptimisation2011(
          const std::shared_ptr<OptimisationProblem> optimisationProblem,
          const unsigned int& populationSize) noexcept;

      StandardParticleSwarmOptimisation2011(const StandardParticleSwarmOptimisation2011&) = delete;
      StandardParticleSwarmOptimisation2011& operator=(const StandardParticleSwarmOptimisation2011&) = delete;

      void setNeighbourhoodProbability(
          const double& neighbourhoodProbability) noexcept;
      void setAcceleration(
          const double& acceleration) noexcept;
      void setLocalAttraction(
          const double& localAttraction) noexcept;
      void setGlobalAttraction(
          const double& globalAttraction) noexcept;

      void setMaximalSwarmConvergence(
          const double& swarmConvergence) noexcept;

      std::string to_string() const noexcept override;

    protected:
      double neighbourhoodProbability_;
      double acceleration_;
      double localAttraction_;
      double globalAttraction_;

      double maximalSwarmConvergence_;

      arma::Mat<double> particles_;
      arma::Mat<double> velocities_;

      arma::Mat<double> localBestSolutions_;
      arma::Row<double> localBestObjectiveValues_;

      bool randomizeTopology_;

      arma::Mat<arma::uword> topology_;

      void optimiseImplementation() noexcept override;

      void initialiseSwarm() noexcept;
  };
}
