#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"

namespace mant {
  namespace krm {
    class ParallelKinematicMachine3PRRR : public KinematicallyRedundantMachines {
     public:
      explicit ParallelKinematicMachine3PRRR();

     protected:
      const arma::mat::fixed<2, 3> redundantJointsPosition_;
      const arma::mat::fixed<2, 3> redundantJointsAngles_;
      const arma::mat::fixed<2, 3> linkLengths_;
      const arma::mat::fixed<2, 3> endEffectorJointsRelativePosition_;
    };
  }
}
