#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp"

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine6PRUS : public RobotModel {
     public:
      const arma::Mat<double>::fixed<2, 6> linkLengths_;
      const arma::Mat<double>::fixed<3, 6> endEffectorJointPositions_;
      const arma::Mat<double>::fixed<2, 6> baseJointRotationAngles_;
      const arma::Mat<double>::fixed<3, 6> redundantJointStartPositions_;
      const arma::Mat<double>::fixed<3, 6> redundantJointEndPositions_;

      explicit ParallelKinematicMachine6PRUS();

      ParallelKinematicMachine6PRUS(
          const ParallelKinematicMachine6PRUS& parallelKinematicMachine6PRUS);

      explicit ParallelKinematicMachine6PRUS(
          const arma::Mat<double>::fixed<2, 6>& linkLengths,
          const arma::Mat<double>::fixed<3, 6>& endEffectorJointPositions,
          const arma::Mat<double>::fixed<2, 6>& baseJointRotationAngles,
          const arma::Mat<double>::fixed<3, 6>& redundantJointStartPositions,
          const arma::Mat<double>::fixed<3, 6>& redundantJointEndPositions);

      std::string toString() const override;

     protected:
      const arma::Mat<double>::fixed<3, 6> redundantJointStartToEndPositions_;
      const arma::Col<arma::uword> redundantJointIndicies_;
      arma::Mat<double> redundantJointRotationAngles_;
      arma::Cube<double>::fixed<3, 6, 3> baseJointRotations_;
      arma::Mat<double>::fixed<3, 6> baseJointNormals_;

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
