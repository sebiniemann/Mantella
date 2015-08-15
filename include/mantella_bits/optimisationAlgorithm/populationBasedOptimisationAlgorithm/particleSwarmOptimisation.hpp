#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationAlgorithm/populationBasedOptimisationAlgorithm.hpp>

namespace mant {
  class ParticleSwarmOptimisation : public PopulationBasedOptimisationAlgorithm {
    public:
      explicit ParticleSwarmOptimisation(
          const std::shared_ptr<OptimisationProblem> optimisationProblem,
          const arma::uword populationSize);

      void setNeighbourhoodProbability(
          const double neighbourhoodProbability);

      void setMaximalAcceleration(
          const double maximalAcceleration);
      void setMaximalLocalAttraction(
          const double maximalLocalAttraction);
      void setMaximalGlobalAttraction(
          const double maximalGlobalAttraction);

      void setMaximalSwarmConvergence(
          const double maximalSwarmConvergence);

      std::string toString() const override;

    protected:
      double neighbourhoodProbability_;

      double maximalAcceleration_;
      double maximalLocalAttraction_;
      double maximalGlobalAttraction_;

      double maximalSwarmConvergence_;

      arma::Mat<double> particles_;
      arma::Mat<double> velocities_;

      arma::uword particleIndex_;
      arma::Col<double> particle_;

      arma::Col<arma::uword> neighbourhoodParticlesIndecies_;
      arma::uword neighbourhoodBestParticleIndex_;

      arma::Col<double> attractionCenter_;

      arma::Mat<double> localBestSolutions_;
      arma::Row<double> localBestSoftConstraintsValues_;
      arma::Row<double> localBestObjectiveValues_;

      bool randomizeTopology_;

      arma::Mat<arma::uword>topology_;

      void optimiseImplementation() override;

      void initialiseSwarm();

      arma::Mat<arma::uword>getRandomNeighbourhoodTopology();

      double getAcceleration();
      arma::Col<double> getVelocity();
  };
}
