#pragma once

#include <armadillo>

namespace hop {
  class RobotModel {
    public:
      virtual arma::Mat<double> getJacobian(const arma::Col<double>& endEffectorPose, const arma::Col<double>& redundantActuationParameters) const = 0;
  };
}
