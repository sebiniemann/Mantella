#pragma once

// HOP
#include <hop_bits/optimisationProblem/kinematicMachine.hpp>

namespace hop {
  namespace robotic {
    class ParallelKinematicMachine_3PRUS : public KinematicMachine {
      public:
        explicit ParallelKinematicMachine_3PRUS() noexcept;
        explicit ParallelKinematicMachine_3PRUS(
            const arma::Mat<double>::fixed<2, 3>& baseJointsAngles,
            const arma::Mat<double>::fixed<3, 3>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<2, 3>& linkLengths,
            const arma::Mat<double>::fixed<3, 3>& redundantJointStarts,
            const arma::Mat<double>::fixed<3, 3>& redundantJointEnds) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine_3PRUS(const ParallelKinematicMachine_3PRUS&) = delete;
        ParallelKinematicMachine_3PRUS& operator=(const ParallelKinematicMachine_3PRUS&) = delete;

        std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

      protected:
        arma::Mat<double>::fixed<3, 3> endEffectorJointsRelative_;
        arma::Mat<double>::fixed<3, 3> linkLengths_;

        arma::Cube<double>::fixed<3, 3, 3> baseJointsRotation_;
        arma::Mat<double>::fixed<3, 3> baseJointsNormal_;

        arma::Mat<double>::fixed<3, 3> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 3> redundantJointEnds_;
        arma::Mat<double>::fixed<3, 3> redundantJointsStartToEnd_;
        arma::Col<arma::uword> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };
  }
}
