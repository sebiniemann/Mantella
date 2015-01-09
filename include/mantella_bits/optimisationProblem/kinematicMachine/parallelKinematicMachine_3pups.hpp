#pragma once

// Mantella
#include <mantella_bits/optimisationProblem/kinematicMachine.hpp>

namespace mant {
  namespace robotic {
    class ParallelKinematicMachine_3PUPS : public KinematicMachine {
      public:
        explicit ParallelKinematicMachine_3PUPS() noexcept;
        explicit ParallelKinematicMachine_3PUPS(
            const arma::Mat<double>::fixed<3, 3>& relativeEndEffectorJoints,
            const arma::Mat<double>::fixed<3, 3>& redundantJointStarts,
            const arma::Mat<double>::fixed<3, 3>& redundantJointEnds,
            const arma::Row<double>::fixed<3>& minimalActiveJointActuations,
            const arma::Row<double>::fixed<3>& maximalActiveJointActuations) noexcept;

        // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
  //      ParallelKinematicMachine_3PUPS(const ParallelKinematicMachine_3PUPS&) = delete;
        ParallelKinematicMachine_3PUPS& operator=(const ParallelKinematicMachine_3PUPS&) = delete;

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
        arma::Mat<double>::fixed<3, 3> endEffectorJointsRelative_;

        arma::Mat<double>::fixed<3, 3> redundantJointStarts_;
        arma::Mat<double>::fixed<3, 3> redundantJointEnds_;

        arma::Row<double>::fixed<3> minimalActiveJointActuations_;
        arma::Row<double>::fixed<3> maximalActiveJointActuations_;

        arma::Mat<double>::fixed<3, 3> redundantJointsStartToEnd_;
        arma::Col<unsigned int> redundantJointIndicies_;
        arma::Mat<double> redundantJointAngles_;
    };
  }
}
