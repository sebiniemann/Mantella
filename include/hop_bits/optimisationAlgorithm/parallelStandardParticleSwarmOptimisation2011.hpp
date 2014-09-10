#pragma once

#include <array>
#include <vector>

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class ParallelStandardParticleSwarmOptimisation2011 : public OptimisationAlgorithm {
    public:
      explicit ParallelStandardParticleSwarmOptimisation2011(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& populationSize);

      ParallelStandardParticleSwarmOptimisation2011(const ParallelStandardParticleSwarmOptimisation2011&) = delete;
      ParallelStandardParticleSwarmOptimisation2011& operator=(const ParallelStandardParticleSwarmOptimisation2011&) = delete;

      void setNeighbourhoodProbability(const double& neighbourhoodProbability);
      void setAcceleration(const double& acceleration);
      void setLocalAttraction(const double& localAttraction);
      void setGlobalAttraction(const double& globalAttraction);
      void setCommunicationSteps(const unsigned int& communicationSteps);

    protected:
      const unsigned int populationSize_;

      arma::Mat<double> particles_;
      arma::Mat<double> velocities_;

      std::array<arma::Mat<double>, 2> localBestParticles_;
      std::array<arma::Row<double>, 2> localBestObjectiveValues_;

      arma::Row<double> globalBestObjectiveValues_;

      double neighbourhoodProbability_;
      double acceleration_;
      double localAttraction_;
      double globalAttraction_;

      unsigned int communicationSteps_;

      std::vector<bool> randomizeTopology_;

      arma::Mat<arma::uword> topology_;

      bool optimiseImplementation() override;
  };
}
