#pragma once

#include <array>
#include <vector>

#include <hop_bits/optimisationAlgorithm/parallelOptimisationAlgorithm.hpp>

namespace hop {
  class ParallelStandardParticleSwarmOptimisation2011 : public ParallelOptimisationAlgorithm {
    public:
      explicit ParallelStandardParticleSwarmOptimisation2011(const std::shared_ptr<OptimisationProblem> optimisationProblem, const unsigned int& localPopulationSize);

      ParallelStandardParticleSwarmOptimisation2011(const ParallelStandardParticleSwarmOptimisation2011&) = delete;
      ParallelStandardParticleSwarmOptimisation2011& operator=(const ParallelStandardParticleSwarmOptimisation2011&) = delete;

      void setNeighbourhoodProbability(const double& neighbourhoodProbability);
      void setAcceleration(const double& acceleration);
      void setLocalAttraction(const double& localAttraction);
      void setGlobalAttraction(const double& globalAttraction);
      void setCommunicationSteps(const unsigned int& communicationSteps);

    protected:
      const unsigned int localPopulationSize_;

      arma::Mat<double> localParticles_;
      arma::Mat<double> localVelocities_;

      arma::Mat<double> localBestSolutions_;
      arma::Row<double> localBestObjectiveValues_;

      double neighbourhoodProbability_;
      double acceleration_;
      double localAttraction_;
      double globalAttraction_;

      unsigned int communicationSteps_;

      bool randomizeTopology_;

      arma::Mat<arma::uword> topology_;

      void parallelOptimiseImplementation() override;
  };
}
