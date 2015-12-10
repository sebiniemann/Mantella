#pragma once

// C++ standard library
#include <string>
#include <memory>

// C++ standard library
#include <deque>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"

namespace mant {
  class ParticleSwarmOptimisation : public OptimisationAlgorithm {
   public:
    explicit ParticleSwarmOptimisation();

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::Mat<double>& initialParameters) override;

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::uword numberOfParticles);
        
    void setNeighbourhoodTopologyFunction(
        std::function<arma::Mat<arma::uword>(const arma::uword numberOfParticles)> neighbourhoodTopologyFunction);

    void setMaximalAcceleration(
        const double maximalAcceleration);
    void setMaximalLocalAttraction(
        const double maximalLocalAttraction);
    void setMaximalGlobalAttraction(
        const double maximalGlobalAttraction);

   protected:
    std::function<arma::Mat<arma::uword>(const arma::uword numberOfParticles)> neighbourhoodTopologyFunction_;

    double maximalAcceleration_;
    double maximalLocalAttraction_;
    double maximalGlobalAttraction_;

    arma::uword numberOfParticles_;
    arma::uword activeParticleIndex_;
    
    arma::Mat<double> particle_;
    arma::Mat<double> velocities_;
    
    bool randomiseTopology_;
    arma::Mat<arma::uword> neighbourhoodTopology_;

    arma::Mat<double> localBestSolutions_;
    arma::Col<double> localBestObjectiveValues_;
  };
}
