namespace mant {
  // Generates a 2-dimensional right-handside rotation matrix.
  inline arma::Mat<double>::fixed<2, 2> get2DRotationMatrix(
      // Rotation around the x-axis.
      const double& angle) noexcept;

  // Generates a 3-dimensional right-handside rotation matrix.
  inline arma::Mat<double>::fixed<3, 3> get3DRotationMatrix(
      // Rotation around the x-axis.
      const double& rollAngle,
      // Rotation around the y-axis.
      const double& pitchAngle,
      // Rotation around the z-axis.
      const double& yawAngle) noexcept;

  // Calculates the (lower) intersection points between two circles in a 2-dimensional space.
  // Note: Based on the usage of this helper function, we only considers cases with exactly two
  // intersactions a valid.
  inline arma::Col<double>::fixed<2> getCircleCircleIntersection(
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
  inline arma::Col<double>::fixed<3> getCircleSphereIntersection(
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

  //
  // Implementation
  //

  inline arma::Mat<double>::fixed<2, 2> get2DRotationMatrix(
      const double& angle) noexcept {
    double sineAngle = std::sin(angle);
    double cosineAngle = std::cos(angle);

    return arma::Mat<double>::fixed<2, 2>({
      cosineAngle, -sineAngle,
      sineAngle, cosineAngle
    });
  }

  inline arma::Mat<double>::fixed<3, 3> get3DRotationMatrix(
      const double& rollAngle,
      const double& pitchAngle,
      const double& yawAngle) noexcept {
    double sineRollAngle = std::sin(rollAngle);
    double cosineRollAngle = std::cos(rollAngle);
    double sinePitchAngle = std::sin(pitchAngle);
    double cosinePitchAngle = std::cos(pitchAngle);
    double sineYawAngle = std::sin(yawAngle);
    double cosineYawAngle = std::cos(yawAngle);

    // Avoids Rz*Ry*Rx, as this will suffer from singularities.
    return arma::Mat<double>::fixed<3, 3>({
      cosineYawAngle * cosinePitchAngle, cosineYawAngle * sinePitchAngle * sineRollAngle - sineYawAngle * cosineRollAngle, cosineYawAngle * sinePitchAngle * cosineRollAngle + sineYawAngle * sineRollAngle,
      sineYawAngle * cosinePitchAngle, sineYawAngle * sinePitchAngle * sineRollAngle + cosineYawAngle * cosineRollAngle, sineYawAngle * sinePitchAngle * cosineRollAngle - cosineYawAngle * sineRollAngle,
      -sinePitchAngle, cosinePitchAngle * sineRollAngle, cosinePitchAngle * cosineRollAngle
    });
  }

  inline arma::Col<double>::fixed<2> getCircleCircleIntersection(
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

  inline arma::Col<double>::fixed<3> getCircleSphereIntersection(
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
