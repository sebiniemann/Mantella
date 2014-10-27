#pragma once

#include <hop_bits/optimisationProblem/robotic/robotModel.hpp>

namespace hop {
  class ParallelKinematicMachine_6PRUS : public RobotModel {
    public:
      explicit ParallelKinematicMachine_6PRUS();
      explicit ParallelKinematicMachine_6PRUS(const arma::Mat<double>::fixed<3, 6>& relativeEndEffectorJoints, const arma::Mat<double>::fixed<3, 6>& linkLengths, const arma::Mat<double>::fixed<3, 6>& redundantJointStarts, const arma::Mat<double>::fixed<3, 6>& redundantJointEnds);

      arma::Mat<double> getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const override;

    protected:
      arma::Mat<double>::fixed<3, 6> endEffectorJointsRelative_;
      arma::Mat<double>::fixed<3, 6> linkLengths_;

      arma::Mat<double>::fixed<3, 6> redundantJointStarts_;
      arma::Mat<double>::fixed<3, 6> redundantJointEnds_;
      arma::Mat<double>::fixed<3, 6> redundantJointsStartToEnd_;
      arma::Col<arma::uword> redundantJointIndicies_;
      arma::Col<double> redundantJointAnglesSine_;
      arma::Col<double> redundantJointAnglesCosine_;
  };
}
