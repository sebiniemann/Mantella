#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  namespace krm {
    class KinematicallyRedundantMachines : public OptimisationProblem {
     public:
      const arma::uword numberOfWorkspaceDimensions_;

      explicit KinematicallyRedundantMachines(
          const arma::uword numberOfProblemDimensions,
          const arma::uword numberOfWorkspaceDimensions);

      void setEndEffectorTrajectory(
          const arma::mat& endEffectorTrajectory);

      arma::mat getEndEffectorTrajectory() const;

     protected:
      arma::mat endEffectorTrajectory_;
    };
  }
}
