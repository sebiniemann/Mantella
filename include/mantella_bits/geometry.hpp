#pragma once

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  arma::Mat<double>::fixed<2, 2> rotationMatrix2D(
      const double angle);

  arma::Mat<double>::fixed<3, 3> rotationMatrix3D(
      const double rollAngle,
      const double pitchAngle,
      const double yawAngle);

  std::vector<arma::Col<double>::fixed<2>> circleCircleIntersections(
      const arma::Col<double>::fixed<2>& firstCenter,
      const double firstRadius,
      const arma::Col<double>::fixed<2>& secondCenter,
      const double secondRadius);

  std::vector<arma::Col<double>::fixed<3>> circleSphereIntersections(
      const arma::Col<double>::fixed<3>& circleCenter,
      const double circleRadius,
      const arma::Col<double>::fixed<3>& circleNormal,
      const arma::Col<double>::fixed<3>& sphereCenter,
      const double sphereRadius);
}
