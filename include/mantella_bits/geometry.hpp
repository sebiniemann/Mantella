#pragma once

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

namespace mant {
  arma::mat::fixed<2, 2> rotationMatrix2d(
      const double angle);

  arma::mat::fixed<3, 3> rotationMatrix3d(
      const double rollAngle,
      const double pitchAngle,
      const double yawAngle);

  std::vector<arma::vec::fixed<2>> circleCircleIntersections(
      const arma::vec::fixed<2>& firstCenter,
      const double firstRadius,
      const arma::vec::fixed<2>& secondCenter,
      const double secondRadius);

  std::vector<arma::vec::fixed<3>> circleSphereIntersections(
      const arma::vec::fixed<3>& circleCenter,
      const double circleRadius,
      const arma::vec::fixed<3>& circleNormal,
      const arma::vec::fixed<3>& sphereCenter,
      const double sphereRadius);
}
