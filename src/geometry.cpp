#include "mantella_bits/geometry.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  arma::Mat<double>::fixed<2, 2> rotationMatrix2D(
      const double angle) {
    // No need to calculate the sine or cosine more than once.
    const double sineAngle = std::sin(angle);
    const double cosineAngle = std::cos(angle);

    return arma::Mat<double>::fixed<2, 2>({cosineAngle, -sineAngle, sineAngle, cosineAngle});
  }

  arma::Mat<double>::fixed<3, 3> rotationMatrix3D(
      const double rollAngle,
      const double pitchAngle,
      const double yawAngle) {
    // No need to calculate the sine or cosine of each angle more than once.
    const double sineRollAngle = std::sin(rollAngle);
    const double cosineRollAngle = std::cos(rollAngle);
    const double sinePitchAngle = std::sin(pitchAngle);
    const double cosinePitchAngle = std::cos(pitchAngle);
    const double sineYawAngle = std::sin(yawAngle);
    const double cosineYawAngle = std::cos(yawAngle);

    // Direct calculation of the rotation matrix, avoiding Rz*Ry*Rx, as this will suffer from singularities.
    return arma::Mat<double>::fixed<3, 3>({cosineYawAngle * cosinePitchAngle, cosineYawAngle * sinePitchAngle * sineRollAngle - sineYawAngle * cosineRollAngle, cosineYawAngle * sinePitchAngle * cosineRollAngle + sineYawAngle * sineRollAngle, sineYawAngle * cosinePitchAngle, sineYawAngle * sinePitchAngle * sineRollAngle + cosineYawAngle * cosineRollAngle, sineYawAngle * sinePitchAngle * cosineRollAngle - cosineYawAngle * sineRollAngle, -sinePitchAngle, cosinePitchAngle * sineRollAngle, cosinePitchAngle * cosineRollAngle});
  }

  std::vector<arma::Col<double>::fixed<2>> circleCircleIntersections(
      const arma::Col<double>::fixed<2>& firstCentre,
      const double firstRadius,
      const arma::Col<double>::fixed<2>& secondCentre,
      const double secondRadius) {
    // The circle circle intersection point is calculated as following:
    // 1. Assume that both centers are on the x-axis and *firstCentre* is at (0, 0).
    // 2. Calculate the distance between both centers.
    const double distance = arma::norm(secondCentre - firstCentre);

    if (distance < 1e-12 && std::abs(firstRadius - secondRadius) < 1e-12) {
      verify(firstRadius < 1e-12, "circleCircleIntersections: Both centers and radii (> 0) are identical, resulting in infinite intersections.");

      // Both circles are identical dots.
      return {firstCentre};
    }

    // Returns an empty set (no intersection), if both circles are either to far away or to close.
    if (distance > firstRadius + secondRadius || distance < std::abs(firstRadius - secondRadius)) {
      return {};
    }

    // 3. Calculate the x-coordinate of the intersection point (combining both circle equations and solve for x).
    const double x = (std::pow(firstRadius, 2.0) - std::pow(secondRadius, 2.0) + std::pow(distance, 2.0)) / (2.0 * distance);

    // 4. Generate a unit vector pointing from *firstCentre* to *secondCentre*.
    const arma::Col<double>::fixed<2>& unitVector = (secondCentre - firstCentre) / distance;

    if (std::abs(firstRadius - std::abs(x)) < 1e-12) {
      // One intersection
      // 6. Scale, rotate and translate the (x, y)-coordinate to be within the actual coordinate system (remove the assumption from 1.)
      return {{firstCentre + unitVector * x}};
    } else {
      // Two intersections
      // 5. Calculate the y-coordinate (Pythagoras, using the x-coordinate as adjacent and *firstRadius* as hypotenuse).
      const double y = std::sqrt(std::pow(firstRadius, 2.0) - std::pow(x, 2.0));

      // 6. Scale, rotate and translate the (x, y)-coordinate to be within the actual coordinate system (remove the assumption from 1.)
      return {{firstCentre(0) + unitVector(0) * x - unitVector(1) * y, firstCentre(1) + unitVector(1) * x + unitVector(0) * y}, {firstCentre(0) + unitVector(0) * x + unitVector(1) * y, firstCentre(1) + unitVector(1) * x - unitVector(0) * y}};
    }
  }

  std::vector<arma::Col<double>::fixed<3>> circleSphereIntersections(
      const arma::Col<double>::fixed<3>& circleCentre,
      const double circleRadius,
      const arma::Col<double>::fixed<3>& circleNormal,
      const arma::Col<double>::fixed<3>& sphereCentre,
      const double sphereRadius) {
    // The circle sphere intersection point is calculated as following:
    // 1. Calculate the shortest distance between the sphere's centre and circle's plane.
    const double innerDistance = arma::dot(circleNormal, circleCentre - sphereCentre);

    // Returns an empty set (no intersection), if the circle's plane does not intersect with the sphere.
    if (std::abs(innerDistance) > sphereRadius + 1e-12) {
      return {};
    };

    // 2. Calculate the centre of the sphere's circle segment, placed an the same plane as the given circle.
    const arma::Col<double>::fixed<3>& innerCentre = sphereCentre + innerDistance * circleNormal;
    // 3. Calculate the radius of the sphere's circle segment.
    // Due to rounding errors, the inner result might be negative instead of being 0.
    const double innerRadius = std::sqrt(std::max(0.0, std::pow(sphereRadius, 2.0) - std::pow(innerDistance, 2.0)));

    // Given the provided circle and the sphere's circle (both circle are on the same plane), the problem is reduced to a circle circle intersection.
    // 4. We now assume that both centers are on the x-axis, *circleNormal* is perpendicular to the x- and y-axis and *firstCentre* is at (0, 0, 0).
    const double distance = arma::norm(innerCentre - circleCentre);
    if (distance < 1e-12 && std::abs(innerRadius - circleRadius) < 1e-12) {
      verify(circleRadius < 1e-12, "circleSphereIntersections: Both centers and radii (> 0) are identical, resulting in infinite intersections.");

      // Both circles are identical dots.
      return {circleCentre};
    }

    // Returns an empty set (no intersection), if both circles are either to far away or to close.
    if (distance > circleRadius + innerRadius + 1e-12 || distance < std::abs(circleRadius - innerRadius) - 1e-12) {
      return {};
    };

    // 5. Calculate the x-coordinate of the intersection point (combining both circle equations and solve for x).
    const double x = (std::pow(circleRadius, 2.0) - std::pow(innerRadius, 2.0) + std::pow(distance, 2.0)) / (2.0 * distance);
    // const double x = (2.0 * (std::pow(circleRadius, 2.0) - std::pow(innerRadius, 2.0)) + distance) / (2.0 + 2.0 * distance);
    // 6. Generate a unit vector pointing from *circleCentre* to *innerCentre*.
    const arma::Col<double>::fixed<3>& xUnitVector = (innerCentre - circleCentre) / distance;

    if (std::abs(circleRadius - std::abs(x)) < 1e-12) {
      // One intersection
      // 7. Scale and translate the unit vector.
      return {circleCentre + x * xUnitVector};
    } else {
      // Two intersections
      // 7. Calculate the y-coordinate (Pythagoras, using the x-coordinate as adjacent and *circleRadius* as hypotenuse).
      const double y = std::sqrt(std::pow(circleRadius, 2.0) - std::pow(x, 2.0));
      // 8. Generate a second unit vector, perpendicular to *xUnitVector* and *circleNormal*.
      const arma::Col<double>::fixed<3>& yUnitVector = arma::normalise(arma::cross(xUnitVector, circleNormal));

      // 9. Scale and translate both unit vectors.
      return {circleCentre + x * xUnitVector + y * yUnitVector, circleCentre + x * xUnitVector - y * yUnitVector};
    }
  }
}
