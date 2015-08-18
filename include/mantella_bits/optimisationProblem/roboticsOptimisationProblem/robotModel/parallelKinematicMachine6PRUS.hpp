#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine6PRUS : public RobotModel {
      public:
        explicit ParallelKinematicMachine6PRUS(
            const arma::uword numberOfRedundantJoints);
            
        void setLinkLengths(
            arma::Mat<double>::fixed<2, 6> linkLengths);
        void setEndEffectorJointPositions(
            arma::Mat<double>::fixed<3, 6> endEffectorJointPositions);
        void setBaseJointRotationAngles(
            arma::Mat<double>::fixed<2, 6> baseJointRotationAngles);
        void setRedundantJointStartPositions(
            arma::Mat<double>::fixed<3, 6> redundantJointStartPositions);
        void setRedundantJointEndPositions(
            arma::Mat<double>::fixed<3, 6> redundantJointEndPositions);
            
        arma::Mat<double>::fixed<2, 6> getLinkLengths() const;
        arma::Mat<double>::fixed<3, 6> getEndEffectorJointPositions() const;
        arma::Mat<double>::fixed<2, 6> getBaseJointRotationAngles() const;
        arma::Mat<double>::fixed<3, 6> getRedundantJointStartPositions() const;
        arma::Mat<double>::fixed<3, 6> getRedundantJointEndPositions() const;
            
        std::string toString() const;

      protected:
        arma::Mat<double>::fixed<2, 6> linkLengths_;
        arma::Mat<double>::fixed<3, 6> endEffectorJointPositions_;
        arma::Mat<double>::fixed<2, 6> baseJointRotationAngles_;
        arma::Mat<double>::fixed<3, 6> redundantJointStartPositions_;
        arma::Mat<double>::fixed<3, 6> redundantJointEndPositions_;
        

        arma::Mat<double>::fixed<3, 6> redundantJointStartToEndPositions_;
        arma::Cube<double>::fixed<3, 6, 3> baseJointRotations_;
        arma::Mat<double>::fixed<3, 6> baseJointNormals_;
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