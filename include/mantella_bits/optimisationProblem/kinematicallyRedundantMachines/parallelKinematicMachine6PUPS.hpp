#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachines.hpp"

namespace mant {
  namespace krm {
    class ParallelKinematicMachine6PUPS : public KinematicallyRedundantMachines {
     public:
      explicit ParallelKinematicMachine6PUPS();

     protected:
      const arma::Mat<double>::fixed<3, 6> redundantJointsPosition_;
      const arma::Mat<double>::fixed<3, 6> redundantJointsAngles_;
      const arma::Row<double>::fixed<6> middleJointsMinimalLength_;
      const arma::Row<double>::fixed<6> middleJointsMaximalLength_;
      const arma::Mat<double>::fixed<3, 6> endEffectorJointsRelativePosition_;
    };
  }
}
