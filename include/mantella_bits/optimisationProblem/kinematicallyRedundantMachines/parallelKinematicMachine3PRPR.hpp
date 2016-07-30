#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"

namespace mant {
  namespace krm {
    class ParallelKinematicMachine3PRPR : public KinematicallyRedundantMachines {
     public:
      explicit ParallelKinematicMachine3PRPR();

     protected:
      const arma::mat::fixed<2, 3> redundantJointsPosition_;
      const arma::mat::fixed<2, 3> redundantJointsAngles_;
      const arma::rowvec::fixed<3> middleJointsMinimalLength_;
      const arma::rowvec::fixed<3> middleJointsMaximalLength_;
      const arma::mat::fixed<2, 3> endEffectorJointsRelativePosition_;
    };
  }
}
