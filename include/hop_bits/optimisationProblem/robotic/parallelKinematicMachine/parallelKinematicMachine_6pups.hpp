#pragma once

#include <hop_bits/optimisationProblem/robotic/parallelKinematicMachine.hpp>

namespace hop {
  class ParallelKinematicMachine_6PUPS : public ParallelKinematicMachine {
    public:
      explicit ParallelKinematicMachine_6PUPS(const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<3, 6>& redundantJointStarts, const arma::Mat<double>::fixed<3, 6>& redundantJointEnds);

      arma::Mat<double>::fixed<3, 6> getRelativeEndEffectorJoints() const;
      arma::Mat<double>::fixed<3, 6> getRedundantJointStarts() const;
      arma::Mat<double>::fixed<3, 6> getRedundantJointEnds() const;

      arma::Mat<double> getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const override;

    protected:
      arma::Mat<double>::fixed<3, 6> endEffectorJointsRelative_;

      arma::Mat<double>::fixed<3, 6> redundantJointStarts_;
      arma::Mat<double>::fixed<3, 6> redundantJointEnds_;
      arma::Mat<double>::fixed<3, 6> redundantJointStartToEnd_;
      arma::Col<arma::uword> redundantJointIndicies_;
  };
}
