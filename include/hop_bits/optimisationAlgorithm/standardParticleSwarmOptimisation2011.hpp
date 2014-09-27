#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class StandardParticleSwarmOptimisation2011 : public OptimisationAlgorithm {
    public:
      explicit StandardParticleSwarmOptimisation2011(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize);

      StandardParticleSwarmOptimisation2011(const StandardParticleSwarmOptimisation2011&) = delete;
      StandardParticleSwarmOptimisation2011& operator=(const StandardParticleSwarmOptimisation2011&) = delete;

      void setNeighbourhoodProbability(const double& neighbourhoodProbability);
      void setAcceleration(const double& acceleration);
      void setLocalAttraction(const double& localAttraction);
      void setGlobalAttraction(const double& globalAttraction);

    protected:
      const unsigned int populationSize_;

      arma::Mat<double> particles_;
      arma::Mat<double> velocities_;

      arma::Mat<double> localBestParticles_;
      arma::Row<double> localBestObjectiveValues_;

      double neighbourhoodProbability_;
      double acceleration_;
      double localAttraction_;
      double globalAttraction_;

      bool randomizeTopology_;

      arma::Mat<arma::uword> topology_;

      void optimiseImplementation() override;
  };
}
