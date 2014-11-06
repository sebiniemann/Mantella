#include <hop_bits/helper/geometry.hpp>

// C++ Standard Library
#include <stdexcept>

namespace hop {
  arma::Mat<double>::fixed<2, 2> Geometry::get2DRotationMatrix(
      const double& angle) {
    double sinAngle = std::sin(angle);
    double cosAngle = std::cos(angle);

    return arma::Mat<double>::fixed<2, 2>({
      cosAngle, -sinAngle,
      sinAngle,  cosAngle
    });
  }

  // TODO Left or right hand side implementation?
  arma::Mat<double>::fixed<3, 3> Geometry::get3DRotationMatrix(
      const double& rollAngle,
      const double& pitchAngle,
      const double& yawAngle) {
    double sinRollAngle = std::sin(rollAngle);
    double cosRollAngle = std::cos(rollAngle);
    double sinPitchAngle = std::sin(pitchAngle);
    double cosPitchAngle = std::cos(pitchAngle);
    double sinYawAngle = std::sin(yawAngle);
    double cosYawAngle = std::cos(yawAngle);

    // Avoids Rz*Ry*Rx, as this will suffer from singularities.
    return arma::Mat<double>::fixed<3, 3>({
      cosYawAngle * cosPitchAngle, cosYawAngle * sinPitchAngle * sinRollAngle - sinYawAngle * cosRollAngle, cosYawAngle * sinPitchAngle * cosRollAngle + sinYawAngle * sinRollAngle,
      sinYawAngle * cosPitchAngle, sinYawAngle * sinPitchAngle * sinRollAngle + cosYawAngle * cosRollAngle, sinYawAngle * sinPitchAngle * cosRollAngle - cosYawAngle * sinRollAngle,
                   -sinPitchAngle,                                            cosPitchAngle * sinRollAngle,                                            cosPitchAngle * cosRollAngle
    });
  }

  arma::Col<double>::fixed<2> Geometry::getCircleCircleIntersection(
      const arma::Col<double>::fixed<2>& firstCenter,
      const double& firstRadius,
      const arma::Col<double>::fixed<2>& secondCenter,
      const double& secondRadius) {
    double distance = arma::norm(secondCenter - firstCenter);

    if (distance == 0 || distance >= firstRadius + secondRadius || distance <= std::max(firstRadius, secondRadius) - std::min(firstRadius, secondRadius)) {
      throw std::runtime_error("Only intersections with exactly two intersections are considered valid.");
    }

    double cosine = (std::pow(firstRadius, 2) - std::pow(secondRadius, 2) + std::pow(distance, 2)) / (2 * distance);
    double sine = std::sqrt(std::pow(firstRadius, 2) - std::pow(cosine, 2));

    arma::Col<double>::fixed<2> normal = arma::normalise(secondCenter - firstCenter);

    return firstCenter + arma::Col<double>::fixed<2>({
      normal.at(0) * cosine + normal.at(1) * sine,
      normal.at(1) * cosine - normal.at(0) * sine
    });
  }

  arma::Col<double>::fixed<3> Geometry::getCircleSphereIntersection(
      const arma::Col<double>::fixed<3>& circleCenter,
      const double& circleRadius,
      const arma::Col<double>::fixed<3>& circleNormal,
      const arma::Col<double>::fixed<3>& sphereCenter,
      const double& sphereRadius) {
    // Distance between the spheres center and the intersection circle within the sphere
    double innerDistance = arma::dot(circleNormal, sphereCenter - circleCenter);

    if (std::abs(innerDistance) >= sphereRadius) {
      throw std::runtime_error("Only intersections with exactly two intersections are considered valid.");
    }

    arma::Col<double>::fixed<3> innerCenter = sphereCenter + innerDistance * circleNormal;
    double innerRadius = std::sqrt(std::pow(sphereRadius, 2) - std::pow(innerDistance, 2));
    arma::Col<double>::fixed<3> normal = arma::normalise(arma::cross(innerCenter - circleCenter, circleNormal));

    double distance = arma::norm(innerCenter - circleCenter);
    double intersectionDistance = (std::pow(circleRadius, 2) - std::pow(innerRadius, 2) + std::pow(distance, 2)) / (2 * distance);

    return circleCenter + intersectionDistance / distance * (innerCenter - circleCenter) + normal * std::sqrt(std::pow(circleRadius, 2) - std::pow(intersectionDistance, 2));
  }
}
