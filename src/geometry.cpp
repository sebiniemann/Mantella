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

  arma::Mat<double>::fixed<2, 2> circleCircleIntersections(
      const arma::Col<double>::fixed<2>& firstCentre,
      const double firstRadius,
      const arma::Col<double>::fixed<2>& secondCentre,
      const double secondRadius) {
    verify(firstRadius > 0, "circleCircleIntersections: The radius of the first circle must be strict greater than 0.");
    verify(secondRadius > 0, "circleCircleIntersections: The radius of the second circle must be strict greater than 0.");

    // The circle circle intersection point is calculated as following:
    // 1. Assume that both centres are on the x-axis and *firstCentre* is at (0, 0).
    // 2. Calculate the distance between both centres.
    const double distance = arma::norm(secondCentre - firstCentre);
    verify(distance > 0, "circleCircleIntersections: The distance between both circle centres must be strict greater than 0.");
    verify(distance <= firstRadius + secondRadius, "circleCircleIntersections: The distance between both circles centres must be less than the sum of their radii.");
    verify(distance >= std::max(firstRadius, secondRadius) - std::min(firstRadius, secondRadius), "circleCircleIntersections: The distance between both centres must be strict greater than the difference in their radii.");

    // 3. Calculate the x-coordinate of the intersection point (combining both circle equations and solve for x).
    const double x = (std::pow(firstRadius, 2.0) - std::pow(secondRadius, 2.0) + std::pow(distance, 2.0)) / (2.0 * distance);
    // 4. Calculate the y-coordinate (Pythagoras, using the x-coordinate as adjacent and *firstRadius* as hypotenuse).
    const double y = std::sqrt(std::pow(firstRadius, 2.0) - std::pow(x, 2.0));

    // 5. Generate a unit vector pointing from *firstCentre* to *secondCentre*.
    const arma::Col<double>::fixed<2>& unitVector = (secondCentre - firstCentre) / distance;

    // 6. Scale, rotate and translate the (x, y)-coordinate to be within the actual coordinate system (remove the assumption from 1.)
    return arma::Mat<double>::fixed<2, 2>({
        firstCentre(0) + unitVector(0) * x - unitVector(1) * y, firstCentre(1) + unitVector(1) * x + unitVector(0) * y, firstCentre(0) + unitVector(0) * x + unitVector(1) * y, firstCentre(1) + unitVector(1) * x - unitVector(0) * y});
  }

  arma::Mat<double>::fixed<3, 2> circleSphereIntersections(
      const arma::Col<double>::fixed<3>& circleCentre,
      const double circleRadius,
      const arma::Col<double>::fixed<3>& circleNormal,
      const arma::Col<double>::fixed<3>& sphereCentre,
      const double sphereRadius) {
    verify(circleRadius > 0, "circleSphereIntersections: The radius of the circle must be strict greater than 0.");
    verify(sphereRadius > 0, "circleSphereIntersections: The radius of the sphere must be strict greater than 0.");

    // The circle sphere intersection point is calculated as following:
    // 1. Calculate the shortest distance between the sphere's centre and circle's plane.
    const double innerDistance = arma::dot(circleNormal, sphereCentre - circleCentre);
    verify(std::abs(innerDistance) < sphereRadius, "circleSphereIntersections: The sphere and the circle does not intersect.");

    // 2. Calculate the centre of the sphere's circle segment, placed an the same plane as the given circle.
    const arma::Col<double>::fixed<3>& innerCentre = sphereCentre + innerDistance * circleNormal;
    // 3. Calculate the radius of the sphere's circle segment.
    const double innerRadius = std::sqrt(std::pow(sphereRadius, 2.0) - std::pow(innerDistance, 2.0));

    // Given the provided circle and the sphere's circle (both circle are on the same plane), the problem is reduced to a circle circle intersection.
    // 4. We now assume that both centres are on the x-axis, *circleNormal* is perpendicular to the x- and y-axis and *firstCentre* is at (0, 0, 0).
    const double distance = arma::norm(innerCentre - circleCentre);
    verify(distance > 0, "circleSphereIntersections: The distance between both circle centres must be strict greater than 0. Note: The second circle is defined by a plane/sphere intersection.");
    verify(distance <= circleRadius + innerRadius, "circleSphereIntersections: The distance between both circles centres must be strict less than the sum of their radii. Note: The second circle is defined by a plane/sphere intersection.");
    verify(distance >= std::max(circleRadius, innerRadius) - std::min(circleRadius, innerRadius), "circleSphereIntersections: The distance between both circles centres must be strict greater than the difference in their radii. Note: The second circle is defined by a plane/sphere intersection.");

    // 5. Calculate the x-coordinate of the intersection point (combining both circle equations and solve for x).
    const double x = (std::pow(circleRadius, 2.0) - std::pow(innerRadius, 2.0) + std::pow(distance, 2.0)) / (2.0 * distance);
    // 6. Calculate the y-coordinate (Pythagoras, using the x-coordinate as adjacent and *circleRadius* as hypotenuse).
    const double y = std::sqrt(std::pow(circleRadius, 2.0) - std::pow(x, 2.0));

    // 7. Generate a unit vector pointing from *circleCentre* to *innerCentre*.
    const arma::Col<double>::fixed<3>& xUnitVector = (innerCentre - circleCentre) / distance;
    // 8. Generate a second unit vector, perpendicular to *xUnitVector* and *circleNormal*.
    const arma::Col<double>::fixed<3>& yUnitVector = arma::normalise(arma::cross(xUnitVector, circleNormal));

    // 9. Scale and translate both unit vectors.
    arma::Mat<double>::fixed<3, 2> intersections;
    intersections.col(0) = circleCentre + x * xUnitVector + y * yUnitVector;
    intersections.col(1) = circleCentre + x * xUnitVector - y * yUnitVector;
    return intersections;
  }
}
