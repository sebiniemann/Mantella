#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel.hpp"

namespace mant {
  namespace krmp {
    class ParallelKinematicMachine3PRPR : public MachineModel {
     public:
      const arma::Cube<double>::fixed<2, 3, 2> redundantJoints_;
      const arma::Mat<double>::fixed<2, 3> middleJointLimitations_;
      const arma::Mat<double>::fixed<2, 3> relativeEndEffectorJoints_;

      explicit ParallelKinematicMachine3PRPR();

      // Adds a copy constructor to provide support for *std::array* and similar containers.
      // *Note:* Don't mark this constructor with the *explicit* keyword.
      ParallelKinematicMachine3PRPR(
          const ParallelKinematicMachine3PRPR& parallelKinematicMachine3PRPR);

      explicit ParallelKinematicMachine3PRPR(
          const arma::Cube<double>::fixed<2, 3, 2>& redundantJoints,
          const arma::Mat<double>::fixed<2, 3>& middleJointLimitations,
          const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints);

     protected:
      const arma::Mat<double>::fixed<2, 3> redundantJointDirections_;
      // The size of these fields depend on the number of redundant joints.
      const arma::Col<arma::uword> redundantJointIndicies_;
      const arma::Mat<double> redundantJointAngles_;

      arma::Mat<double> getRedundantJointAngles(
          const arma::Mat<double>::fixed<2, 3> redundantJointDirections,
          const arma::Col<arma::uword> redundantJointIndicies) const;
    };
  }
}
