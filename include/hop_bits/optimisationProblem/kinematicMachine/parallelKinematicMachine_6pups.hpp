#pragma once

// HOP
#include <hop_bits/optimisationProblem/kinematicMachine.hpp>

namespace hop {
  namespace robotic {
    class ParallelKinematicMachine_6PUPS : public KinematicMachine {
      public:
        explicit ParallelKinematicMachine_6PUPS() noexcept;
        explicit ParallelKinematicMachine_6PUPS(
            const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<3, 6>& redundantJointStarts,
            const arma::Mat<double>::fixed<3, 6>& redundantJointEnds,
            const arma::Row<double>::fixed<6>& minimalActiveJointActuations,
            const arma::Row<double>::fixed<6>& maximalActiveJointActuations) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
  //      ParallelKinematicMachine_6PUPS(const ParallelKinematicMachine_6PUPS&) = delete;
        ParallelKinematicMachine_6PUPS& operator=(const ParallelKinematicMachine_6PUPS&) = delete;

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
        arma::Mat<double>::fixed<3, 6> endEffectorJointsRelative_;

        arma::Mat<double>::fixed<3, 6> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 6> redundantJointEnds_;

        arma::Row<double>::fixed<6> minimalActiveJointActuations_;
        arma::Row<double>::fixed<6> maximalActiveJointActuations_;

        arma::Mat<double>::fixed<3, 6> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };
  }
}
