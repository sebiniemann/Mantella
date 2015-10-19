#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp"

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine3PRUS : public RobotModel {
     public:
      const arma::Mat<double>::fixed<2, 3> linkLengths_;
      const arma::Mat<double>::fixed<3, 3> endEffectorJointPositions_;
      const arma::Mat<double>::fixed<2, 3> baseJointRotationAngles_;
      const arma::Mat<double>::fixed<3, 3> redundantJointStartPositions_;
      const arma::Mat<double>::fixed<3, 3> redundantJointEndPositions_;

      explicit ParallelKinematicMachine3PRUS();

      ParallelKinematicMachine3PRUS(
          const ParallelKinematicMachine3PRUS& parallelKinematicMachine3PRUS);

      explicit ParallelKinematicMachine3PRUS(
          const arma::Mat<double>::fixed<2, 3>& linkLengths,
          const arma::Mat<double>::fixed<3, 3>& endEffectorJointPositions,
          const arma::Mat<double>::fixed<2, 3>& baseJointRotationAngles,
          const arma::Mat<double>::fixed<3, 3>& redundantJointStartPositions,
          const arma::Mat<double>::fixed<3, 3>& redundantJointEndPositions);

      std::string toString() const override;

     protected:
      const arma::Mat<double>::fixed<3, 3> redundantJointStartToEndPositions_;
      const arma::Col<arma::uword> redundantJointIndicies_;
      arma::Mat<double> redundantJointRotationAngles_;
      arma::Cube<double>::fixed<3, 3, 3> baseJointRotations_;
      arma::Mat<double>::fixed<3, 3> baseJointNormals_;

      arma::Cube<double> getModelImplementation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const override;

      arma::Row<double> getActuationImplementation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const override;

      double getEndEffectorPoseErrorImplementation(
          const arma::Col<double>& endEffectorPose,
          const arma::Row<double>& redundantJointsActuation) const override;
    };
  }
}
