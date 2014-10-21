#pragma once

#include <armadillo>

namespace hop {
  class Geometry {
    public:
      static arma::Mat<double>::fixed<2, 2> get2DRotationMatrix(const double& angle);
      static arma::Mat<double>::fixed<3, 3> get3DRotationMatrix(const double& rollAngle, const double& pitchAngle, const double& yawAngle);

      static arma::Mat<double> getCircleCircleIntersection(const arma::Mat<double>& firstCenter, const arma::Row<double>& firstRadius, const arma::Mat<double>& secondCenter, const arma::Row<double>& secondRadius);
      static arma::Mat<double> getCircleSphereIntersection(const arma::Mat<double>& circleCenter, const arma::Row<double>& circleRadius, const arma::Mat<double>& circleNormal, const arma::Mat<double>& sphereCenter, const arma::Row<double>& sphereRadius);
  };
}
