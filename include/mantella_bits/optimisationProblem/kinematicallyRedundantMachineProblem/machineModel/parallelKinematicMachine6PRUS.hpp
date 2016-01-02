#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel.hpp"

namespace mant {
  namespace krmp {
    class ParallelKinematicMachine6PRUS : public MachineModel {
     public:
      const arma::Cube<double>::fixed<3, 6, 2> redundantJoints_;
      const arma::Mat<double>::fixed<3, 6> baseJointNormals_;
      const arma::Mat<double>::fixed<2, 6> linkLengths_;
      const arma::Mat<double>::fixed<3, 6> relativeEndEffectorJoints_;

      explicit ParallelKinematicMachine6PRUS();

      // Adds a copy constructor to provide support for *std::array* and similar containers.
      // *Note:* Don't mark this constructor with the *explicit* keyword.
      ParallelKinematicMachine6PRUS(
          const ParallelKinematicMachine6PRUS& parallelKinematicMachine6PRUS);

      explicit ParallelKinematicMachine6PRUS(
          const arma::Cube<double>::fixed<3, 6, 2>& redundantJoints,
          const arma::Mat<double>::fixed<3, 6>& baseJointNormals,
          const arma::Mat<double>::fixed<2, 6>& linkLengths,
          const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints);

     protected:
      const arma::Mat<double>::fixed<3, 6> redundantJointDirections_;
      // The size of these fields depend on the number of redundant joints.
      const arma::Col<arma::uword> redundantJointIndicies_;
      const arma::Mat<double> redundantJointAngles_;

      arma::Mat<double> getRedundantJointAngles(
          const arma::Mat<double>::fixed<3, 6> redundantJointDirections,
          const arma::Col<arma::uword> redundantJointIndicies) const;
    };
  }
}
