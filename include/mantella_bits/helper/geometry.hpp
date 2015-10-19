#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double>::fixed<2, 2> get2DRotation(
      const double angle);

  arma::Mat<double>::fixed<3, 3> get3DRotation(
      const double rollAngle,
      const double pitchAngle,
      const double yawAngle);

  arma::Col<double>::fixed<2> getCircleCircleIntersection(
      const arma::Col<double>::fixed<2>& firstCenter,
      const double firstRadius,
      const arma::Col<double>::fixed<2>& secondCenter,
      const double secondRadius);

  arma::Col<double>::fixed<3> getCircleSphereIntersection(
      const arma::Col<double>::fixed<3>& circleCenter,
      const double circleRadius,
      const arma::Col<double>::fixed<3>& circleNormal,
      const arma::Col<double>::fixed<3>& sphereCenter,
      const double sphereRadius);

  arma::Col<double>::fixed<3> getTriangulation(
      const arma::Col<double>::fixed<3>& firstCenter,
      const double firstRadius,
      const arma::Col<double>::fixed<3>& secondCenter,
      const double secondRadius,
      const arma::Col<double>::fixed<3>& thirdCenter,
      const double thirdRadius);
}
