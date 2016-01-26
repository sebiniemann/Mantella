#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"

namespace mant {
  namespace krm {
    class KinematicallyRedundantMachines : public OptimisationProblem {
     public:
      explicit KinematicallyRedundantMachines(
          const arma::uword numberOfDimensions);

      void setEndEffectorTrajectory(
          const arma::Mat<double>& endEffectorTrajectory);
      arma::Mat<double> getEndEffectorTrajectory() const;

     protected:
      arma::Mat<double> endEffectorTrajectory_;
    };
  }
}
