#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/optimisationProblem/roboticsOptimisationProblem/robotModel.hpp>

namespace mant {
  namespace robotics {
    class ParallelKinematicMachine3PRRR : public RobotModel {
      public:
        explicit ParallelKinematicMachine3PRRR();
            
        explicit ParallelKinematicMachine3PRRR(
            const arma::Mat<double>::fixed<2, 3>& linkLengths,
            const arma::Mat<double>::fixed<2, 3>& endEffectorJointPositions,
            const arma::Mat<double>::fixed<2, 3>& redundantJointStartPositions,
            const arma::Mat<double>::fixed<2, 3>& redundantJointEndPositions);
            
        arma::Mat<double>::fixed<2, 3> getLinkLengths() const;
        arma::Mat<double>::fixed<2, 3> getEndEffectorJointPositions() const;
        arma::Mat<double>::fixed<2, 3> getRedundantJointStartPositions() const;
        arma::Mat<double>::fixed<2, 3> getRedundantJointEndPositions() const;
            
        std::string toString() const;

      protected:
        const arma::Mat<double>::fixed<2, 3> linkLengths_;
        const arma::Mat<double>::fixed<2, 3> endEffectorJointPositions_;
        const arma::Mat<double>::fixed<2, 3> redundantJointStartPositions_;
        const arma::Mat<double>::fixed<2, 3> redundantJointEndPositions_;
      
        const arma::Mat<double>::fixed<2, 3> redundantJointStartToEndPositions_;
        const arma::Col<arma::uword> redundantJointIndicies_;
        arma::Row<double> redundantJointAngleSines_;
        arma::Row<double> redundantJointAngleCosines_;
        
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