#pragma once

// C++ standard library
#include <functional>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationAlgorithm.hpp"
namespace mant {
  class OptimisationProblem;
}

namespace mant {
  class ParticleSwarmOptimisation : public OptimisationAlgorithm {
   public:
    explicit ParticleSwarmOptimisation();

    void initialise(
        const arma::uword numberOfDimensions,
        const arma::Mat<double>& initialParameters) override;

    // Adds *optimise(OptimisationProblem& optimisationProblem, const arma::Mat<double>& initialParameters)*
    using OptimisationAlgorithm::optimise;

    void optimise(
        OptimisationProblem& optimisationProblem,
        const arma::uword numberOfParticles);

    void optimise(
        OptimisationProblem& optimisationProblem);

    void setNeighbourhoodTopologyFunction(
        std::function<arma::Mat<arma::uword>()> neighbourhoodTopologyFunction,
        const std::string& neighbourhoodTopologyFunctionName);
    void setNeighbourhoodTopologyFunction(
        std::function<arma::Mat<arma::uword>()> neighbourhoodTopologyFunction);
    std::string getNeighbourhoodTopologyFunctionName() const;

    void setMaximalAcceleration(
        const double maximalAcceleration);
    double getMaximalAcceleration() const;
    void setMaximalLocalAttraction(
        const double maximalLocalAttraction);
    double getMaximalLocalAttraction() const;
    void setMaximalGlobalAttraction(
        const double maximalGlobalAttraction);
    double getMaximalGlobalAttraction() const;

    // The following variables are only in public scope, to be used inside lambdas
    double maximalAcceleration_;
    double maximalLocalAttraction_;
    double maximalGlobalAttraction_;

    arma::uword numberOfParticles_;
    arma::uword activeParticleIndex_;

    arma::Mat<double> particles_;
    arma::Mat<double> velocities_;

    bool randomiseTopology_;
    arma::Mat<arma::uword> neighbourhoodTopology_;

    arma::Mat<double> localBestSolutions_;
    arma::Col<double> localBestObjectiveValues_;

   protected:
    std::function<arma::Mat<arma::uword>()> neighbourhoodTopologyFunction_;
    std::string neighbourhoodTopologyFunctionName_;
  };
}
