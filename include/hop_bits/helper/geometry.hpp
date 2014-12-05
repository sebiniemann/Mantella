#pragma once

// Armadillo
#include <armadillo>

namespace hop {
  // Generates a 2-dimensional right-handside rotation matrix.
  arma::Mat<double>::fixed<2, 2> get2DRotationMatrix(
      // Rotation around the x-axis.
      const double& angle) noexcept;

  // Generates a 3-dimensional right-handside rotation matrix.
  arma::Mat<double>::fixed<3, 3> get3DRotationMatrix(
      // Rotation around the x-axis.
      const double& rollAngle,
      // Rotation around the y-axis.
      const double& pitchAngle,
      // Rotation around the z-axis.
      const double& yawAngle) noexcept;

  // Calculates the (lower) intersection points between two circles in a 2-dimensional space.
  // Note: Based on the usage of this helper function, we only considers cases with exactly two
  // intersactions a valid.
  arma::Col<double>::fixed<2> getCircleCircleIntersection(
      // The center of the first circle.
      const arma::Col<double>::fixed<2>& firstCenter,
      // The radius of the first circle.
      const double& firstRadius,
      // The center of the second circle.
      const arma::Col<double>::fixed<2>& secondCenter,
      // The radius of the second circle.
      const double& secondRadius);

  // Calculates the (lower) intersection points between a circle and a sphere in a 3-dimensional
  // space.
  // Note: Based on the usage of this helper function, we only considers cases with exactly two
  // intersactions a valid.
  arma::Col<double>::fixed<3> getCircleSphereIntersection(
      // The center of the circle.
      const arma::Col<double>::fixed<3>& circleCenter,
      // The radius of the circle.
      const double& circleRadius,
      // A normalised (unit length) vector standing perpendicular to the circle.
      const arma::Col<double>::fixed<3>& circleNormal,
      // The center of the sphere.
      const arma::Col<double>::fixed<3>& sphereCenter,
      // The radius of the sphere.
      const double& sphereRadius);
}
