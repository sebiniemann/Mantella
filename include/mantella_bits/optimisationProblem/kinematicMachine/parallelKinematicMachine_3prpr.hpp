#pragma once

// Mantella
#include <mantella_bits/optimisationProblem/kinematicMachine.hpp>

namespace mant {
  namespace robotic {
    class ParallelKinematicMachine_3PRPR : public KinematicMachine {
      public:
        explicit ParallelKinematicMachine_3PRPR() ;
        explicit ParallelKinematicMachine_3PRPR(
            const arma::Mat<double>::fixed<2, 3>& endEffectorJointRelativePositions,
            const arma::Mat<double>::fixed<2, 3>& redundantJointStartPositions,
            const arma::Mat<double>::fixed<2, 3>& redundantJointEndPositions,
            const arma::Row<double>::fixed<3>& minimalActiveJointActuations,
            const arma::Row<double>::fixed<3>& maximalActiveJointActuations) ;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine_3PRPR(const ParallelKinematicMachine_3PRPR&) = delete;
        ParallelKinematicMachine_3PRPR& operator=(const ParallelKinematicMachine_3PRPR&) = delete;

        std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

        arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

        double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const override;

      protected:
        arma::Mat<double>::fixed<2, 3> endEffectorJointRelativePositions_;

        arma::Mat<double>::fixed<2, 3> redundantJointStartPositions_;
        arma::Mat<double>::fixed<2, 3> redundantJointEndPositions_;

        arma::Row<double>::fixed<6> minimalActiveJointActuations_;
        arma::Row<double>::fixed<6> maximalActiveJointActuations_;

        arma::Mat<double>::fixed<2, 3> redundantJointStartToEndPositions_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Row<double> redundantJointAngleSines_;
        arma::Row<double> redundantJointAngleCosines_;
    };
  }
}
