#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp"

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine4PRUS : public RobotModel {
     public:
      const arma::Mat<double>::fixed<2, 4> linkLengths_;
      const arma::Mat<double>::fixed<3, 4> endEffectorJointPositions_;
      const arma::Mat<double>::fixed<2, 4> baseJointRotationAngles_;
      const arma::Mat<double>::fixed<3, 4> redundantJointStartPositions_;
      const arma::Mat<double>::fixed<3, 4> redundantJointEndPositions_;

      explicit ParallelKinematicMachine4PRUS();

      ParallelKinematicMachine4PRUS(
          const ParallelKinematicMachine4PRUS& parallelKinematicMachine4PRUS);

      explicit ParallelKinematicMachine4PRUS(
          const arma::Mat<double>::fixed<2, 4>& linkLengths,
          const arma::Mat<double>::fixed<3, 4>& endEffectorJointPositions,
          const arma::Mat<double>::fixed<2, 4>& baseJointRotationAngles,
          const arma::Mat<double>::fixed<3, 4>& redundantJointStartPositions,
          const arma::Mat<double>::fixed<3, 4>& redundantJointEndPositions);

      std::string toString() const override;

     protected:
      const arma::Mat<double>::fixed<3, 4> redundantJointStartToEndPositions_;
      const arma::Col<arma::uword> redundantJointIndicies_;
      arma::Mat<double> redundantJointRotationAngles_;
      arma::Cube<double>::fixed<3, 4, 3> baseJointRotations_;
      arma::Mat<double>::fixed<3, 4> baseJointNormals_;

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
