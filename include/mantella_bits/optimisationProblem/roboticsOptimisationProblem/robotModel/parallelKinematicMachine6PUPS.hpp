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
        explicit ParallelKinematicMachine6PUPS();
            
        explicit ParallelKinematicMachine6PUPS(
            const arma::Mat<double>::fixed<3, 6>& endEffectorJointPositions,
            const arma::Row<double>::fixed<6>& minimalActiveJointsActuation,
            const arma::Row<double>::fixed<6>& maximalActiveJointsActuation,
            const arma::Mat<double>::fixed<3, 6>& redundantJointStartPositions,
            const arma::Mat<double>::fixed<3, 6>& redundantJointEndPositions);
            
        arma::Mat<double>::fixed<3, 6> getEndEffectorJointPositions() const;
        arma::Row<double>::fixed<6> getMinimalActiveJointsActuation() const;
        arma::Row<double>::fixed<6> getMaximalActiveJointsActuation() const;
        arma::Mat<double>::fixed<3, 6> getRedundantJointStartPositions() const;
        arma::Mat<double>::fixed<3, 6> getRedundantJointEndPositions() const;
            
        std::string toString() const;

      protected:
        const arma::Mat<double>::fixed<3, 6> endEffectorJointPositions_;
        const arma::Row<double>::fixed<6> minimalActiveJointsActuation_;
        const arma::Row<double>::fixed<6> maximalActiveJointsActuation_;
        const arma::Mat<double>::fixed<3, 6> redundantJointStartPositions_;
        const arma::Mat<double>::fixed<3, 6> redundantJointEndPositions_;

        const arma::Mat<double>::fixed<3, 6> redundantJointStartToEndPositions_;
        const arma::Col<arma::uword> redundantJointIndicies_;
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