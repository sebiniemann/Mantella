#pragma once

// HOP
#include <hop_bits/optimisationProblem/robotic/model.hpp>

namespace hop {
  namespace robotic {
    class ParallelKinematicMachine_4PRUS : public Model {
      public:
        explicit ParallelKinematicMachine_4PRUS();
        explicit ParallelKinematicMachine_4PRUS(
            const arma::Mat<double>::fixed<2, 4>& baseJointsAngles,
            const arma::Mat<double>::fixed<3, 4>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<2, 4>& linkLengths,
            const arma::Mat<double>::fixed<3, 4>& redundantJointStarts,
            const arma::Mat<double>::fixed<3, 4>& redundantJointEnds);

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
        ParallelKinematicMachine_4PRUS(const ParallelKinematicMachine_4PRUS&) = delete;
        ParallelKinematicMachine_4PRUS& operator=(const ParallelKinematicMachine_4PRUS&) = delete;

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
        arma::Mat<double>::fixed<3, 4> endEffectorJointsRelative_;
        arma::Mat<double>::fixed<3, 4> linkLengths_;

        arma::Cube<double>::fixed<3, 3, 4> baseJointsRotation_;
        arma::Mat<double>::fixed<3, 4> baseJointsNormal_;

        arma::Mat<double>::fixed<3, 4> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 4> redundantJointEnds_;
        arma::Mat<double>::fixed<3, 4> redundantJointsStartToEnd_;
        arma::Col<arma::uword> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };
  }
}
