#include "mantella_bits/helper/geometry.hpp"

// C++ standard library
#include <cmath>
#include <algorithm>

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  arma::Mat<double>::fixed<2, 2> get2DRotation(
      const double angle) {
    double sineAngle = std::sin(angle);
    double cosineAngle = std::cos(angle);

    return arma::Mat<double>::fixed<2, 2>({cosineAngle, -sineAngle, sineAngle, cosineAngle});
  }

  arma::Mat<double>::fixed<3, 3> get3DRotation(
      const double rollAngle,
      const double pitchAngle,
      const double yawAngle) {
    double sineRollAngle = std::sin(rollAngle);
    double cosineRollAngle = std::cos(rollAngle);
    double sinePitchAngle = std::sin(pitchAngle);
    double cosinePitchAngle = std::cos(pitchAngle);
    double sineYawAngle = std::sin(yawAngle);
    double cosineYawAngle = std::cos(yawAngle);

    // Avoids Rz*Ry*Rx, as this will suffer from singularities.
    return arma::Mat<double>::fixed<3, 3>({cosineYawAngle * cosinePitchAngle, cosineYawAngle * sinePitchAngle * sineRollAngle - sineYawAngle * cosineRollAngle, cosineYawAngle * sinePitchAngle * cosineRollAngle + sineYawAngle * sineRollAngle, sineYawAngle * cosinePitchAngle, sineYawAngle * sinePitchAngle * sineRollAngle + cosineYawAngle * cosineRollAngle, sineYawAngle * sinePitchAngle * cosineRollAngle - cosineYawAngle * sineRollAngle, -sinePitchAngle, cosinePitchAngle * sineRollAngle, cosinePitchAngle * cosineRollAngle});
  }

  arma::Col<double>::fixed<2> getCircleCircleIntersection(
      const arma::Col<double>::fixed<2>& firstCenter,
      const double firstRadius,
      const arma::Col<double>::fixed<2>& secondCenter,
      const double secondRadius) {
    verify(firstRadius > 0, "The radius of the first circle must be strict greater than 0.");
    verify(secondRadius > 0, "The radius of the second circle must be strict greater than 0.");

    double distance = arma::norm(secondCenter - firstCenter);
    verify(distance > 0, "The distance between both circle centres must be strict greater than 0.");
    verify(distance < firstRadius + secondRadius, "The distance between both circles centres must be strict less than the sum of their radii.");
    verify(distance > std::max(firstRadius, secondRadius) - std::min(firstRadius, secondRadius),
        "The distance between both centres must be strict greater than the difference in their radii.");

    double cosine = (std::pow(firstRadius, 2.0) - std::pow(secondRadius, 2.0) + std::pow(distance, 2.0)) / (2.0 * distance);
    double sine = std::sqrt(std::pow(firstRadius, 2.0) - std::pow(cosine, 2.0));

    arma::Col<double>::fixed<2> normal = arma::normalise(secondCenter - firstCenter);

    return firstCenter + arma::Col<double>::fixed<2>({normal(0) * cosine + normal(1) * sine,
                             normal(1) * cosine - normal(0) * sine});
  }

  arma::Col<double>::fixed<3> getCircleSphereIntersection(
      const arma::Col<double>::fixed<3>& circleCenter,
      const double circleRadius,
      const arma::Col<double>::fixed<3>& circleNormal,
      const arma::Col<double>::fixed<3>& sphereCenter,
      const double sphereRadius) {
    verify(circleRadius > 0, "The radius of the circle must be strict greater than 0.");
    verify(sphereRadius > 0, "The radius of the sphere must be strict greater than 0.");

    // Distance between the spheres center and the intersection circle within the sphere
    const double innerDistance = arma::dot(circleNormal, sphereCenter - circleCenter);
    verify(std::abs(innerDistance) < sphereRadius, "Only intersections with exactly two solutions are considered valid.");

    const arma::Col<double>::fixed<3>& innerCenter = sphereCenter + innerDistance * circleNormal;
    const double innerRadius = std::sqrt(std::pow(sphereRadius, 2.0) - std::pow(innerDistance, 2.0));

    const double distance = arma::norm(innerCenter - circleCenter);
    verify(distance > 0, "The distance between both circle centres must be strict greater than 0. Note: The second circle is defnied by a plane/sphere intesection.");
    verify(distance < circleRadius + innerRadius, "The distance between both circles centres must be strict less than the sum of their radii. Note: The second circle is defnied by a plane/sphere intesection.");
    verify(distance > std::max(circleRadius, innerRadius) - std::min(circleRadius, innerRadius),
        "The distance between both centres must be strict greater than the difference in their radii. Note: The second circle is defnied by a plane/sphere intesection.");

    const arma::Col<double>::fixed<3>& normal = arma::normalise(arma::cross(innerCenter - circleCenter, circleNormal));

    const double intersectionDistance = (std::pow(circleRadius, 2.0) - std::pow(innerRadius, 2.0) + std::pow(distance, 2.0)) / (2.0 * distance);

    return circleCenter + intersectionDistance / distance * (innerCenter - circleCenter) + normal * std::sqrt(std::pow(circleRadius, 2.0) - std::pow(intersectionDistance, 2.0));
  }

  arma::Col<double>::fixed<3> getTriangulation(
      const arma::Col<double>::fixed<3>& firstCenter,
      const double firstRadius,
      const arma::Col<double>::fixed<3>& secondCenter,
      const double secondRadius,
      const arma::Col<double>::fixed<3>& thirdCenter,
      const double thirdRadius) {
    const arma::Col<double>::fixed<3>& firstToSecondCenter = secondCenter - firstCenter;
    const arma::Col<double>::fixed<3>& firstToThirdCenter = thirdCenter - firstCenter;

    const arma::Col<double>::fixed<3>& normal = arma::cross(firstToSecondCenter, firstToThirdCenter);
    const double normalLength = arma::norm(normal);

    const arma::Col<double>::fixed<3>& firstToInnerCenter = (arma::cross((std::pow(arma::norm(firstToSecondCenter), 2.0) + std::pow(firstRadius, 2.0) - std::pow(secondRadius, 2.0)) * firstToThirdCenter - (std::pow(arma::norm(firstToThirdCenter), 2.0) + std::pow(firstRadius, 2.0) - std::pow(thirdRadius, 2.0)) * firstToSecondCenter, normal)) / std::pow(normalLength, 2.0);
    const arma::Col<double>::fixed<3>& innerCenterToIntercection = std::sqrt(std::pow(firstRadius, 2.0) - std::pow(arma::norm(firstToInnerCenter), 2.0)) * normal / normalLength;

    return firstCenter + firstToInnerCenter + innerCenterToIntercection;
  }
}
