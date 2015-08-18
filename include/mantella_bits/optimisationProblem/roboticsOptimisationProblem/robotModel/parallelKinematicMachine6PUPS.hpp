#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine6PUPS : public RobotModel {
      public:
        explicit ParallelKinematicMachine6PUPS(
            const arma::uword numberOfRedundantJoints);
            
        void setEndEffectorJointPositions(
            arma::Mat<double>::fixed<3, 6> endEffectorJointPositions);
        void setRedundantJointStartPositions(
            arma::Mat<double>::fixed<3, 6> redundantJointStartPositions);
        void setRedundantJointEndPositions(
            arma::Mat<double>::fixed<3, 6> redundantJointEndPositions);
            
        arma::Mat<double>::fixed<3, 6> getEndEffectorJointPositions() const;
        arma::Mat<double>::fixed<3, 6> getRedundantJointStartPositions() const;
        arma::Mat<double>::fixed<3, 6> getRedundantJointEndPositions() const;
            
        std::string toString() const;

      protected:
        arma::Mat<double>::fixed<3, 6> endEffectorJointPositions_;
        arma::Mat<double>::fixed<3, 6> redundantJointStartPositions_;
        arma::Mat<double>::fixed<3, 6> redundantJointEndPositions_;

        arma::Mat<double>::fixed<3, 6> redundantJointStartToEndPositions_;
        arma::Col<arma::uword> redundantJointIndicies_;
        arma::Mat<double> redundantJointRotationAngles_;

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