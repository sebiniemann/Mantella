#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"

namespace mant {
  namespace krm {
    class ParallelKinematicMachine6PRUS : public KinematicallyRedundantMachines {
     public:
      const arma::Mat<double>::fixed<3, 6> redundantJointsPosition_;
      const arma::Mat<double>::fixed<3, 6> redundantJointsAngles_;
      const arma::Mat<double>::fixed<3, 6> baseJointsNormal_;
      const arma::Mat<double>::fixed<2, 6> linkLengths_;
      const arma::Mat<double>::fixed<3, 6> endEffectorJointsRelativePosition_;

      explicit ParallelKinematicMachine6PRUS();
    };
  }
}
