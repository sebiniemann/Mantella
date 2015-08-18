#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine4PRUS : public RobotModel {
      public:
        explicit ParallelKinematicMachine4PRUS(
            const arma::uword numberOfRedundantJoints);
            
        void setLinkLengths(
            arma::Mat<double>::fixed<2, 4> linkLengths);
        void setEndEffectorJointPositions(
            arma::Mat<double>::fixed<3, 4> endEffectorJointPositions);
        void setBaseJointRotationAngles(
            arma::Mat<double>::fixed<2, 4> baseJointRotationAngles);
        void setRedundantJointStartPositions(
            arma::Mat<double>::fixed<3, 4> redundantJointStartPositions);
        void setRedundantJointEndPositions(
            arma::Mat<double>::fixed<3, 4> redundantJointEndPositions);
            
        arma::Mat<double>::fixed<2, 4> getLinkLengths() const;
        arma::Mat<double>::fixed<3, 4> getEndEffectorJointPositions() const;
        arma::Mat<double>::fixed<2, 4> getBaseJointRotationAngles() const;
        arma::Mat<double>::fixed<3, 4> getRedundantJointStartPositions() const;
        arma::Mat<double>::fixed<3, 4> getRedundantJointEndPositions() const;
            
        std::string toString() const;

      protected:
        arma::Mat<double>::fixed<2, 4> linkLengths_;
        arma::Mat<double>::fixed<3, 4> endEffectorJointPositions_;
        arma::Mat<double>::fixed<2, 4> baseJointRotationAngles_;
        arma::Mat<double>::fixed<3, 4> redundantJointStartPositions_;
        arma::Mat<double>::fixed<3, 4> redundantJointEndPositions_;
        

        arma::Mat<double>::fixed<3, 4> redundantJointStartToEndPositions_;
        arma::Cube<double>::fixed<3, 4, 3> baseJointRotations_;
        arma::Mat<double>::fixed<3, 4> baseJointNormals_;
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