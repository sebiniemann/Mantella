#pragma once

// HOP
#include <hop_bits/optimisationProblem/kinematicMachine.hpp>

namespace hop {
  namespace robotic {
    class ParallelKinematicMachine_3PRRR : public KinematicMachine {
      public:
        explicit ParallelKinematicMachine_3PRRR() noexcept;
        explicit ParallelKinematicMachine_3PRRR(
            const arma::Mat<double>::fixed<2, 3>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<2, 3>& linkLengths,
            const arma::Mat<double>::fixed<2, 3>& redundantJointStarts,
            const arma::Mat<double>::fixed<2, 3>& redundantJointEnds) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine_3PRRR(const ParallelKinematicMachine_3PRRR&) = delete;
        ParallelKinematicMachine_3PRRR& operator=(const ParallelKinematicMachine_3PRRR&) = delete;

        std::vector<arma::Mat<double>> getModelCharacterisation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        arma::Mat<double> getActuation(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        double getPositionError(
            const arma::Col<double>& endEffectorPose,
            const arma::Mat<double>& redundantJointActuations) const noexcept override;

        arma::Mat<double>::fixed<2, 3> endEffectorJointsRelative_;
        arma::Mat<double>::fixed<2, 3> linkLengths_;

        arma::Mat<double>::fixed<2, 3> redundantJointStarts_;
        arma::Mat<double>::fixed<2, 3> redundantJointEnds_;
        arma::Mat<double>::fixed<2, 3> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Col<double> redundantJointAnglesSine_;
        arma::Col<double> redundantJointAnglesCosine_;
    };
  }
}
