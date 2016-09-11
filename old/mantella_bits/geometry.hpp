#pragma once

// Armadillo
#include <armadillo>

namespace mant {
  template <typename T>
  std::array<T, 4> rotationMatrix2d(
      const T angle);

  template <typename T1, typename T2, typename T3>
  std::array<std::common_type<T1, T2, T3>, 9> rotationMatrix3d(
      const T1 rollAngle,
      const T2 pitchAngle,
      const T3 yawAngle);

  template <typename T1, typename T2, typename T3, typename T4>
  std::pair<std::vector<std::array<std::common_type<T1, T2, T3, T4>, 2>>, bool> circleCircleIntersections(
      const std::array<T1, 2>& firstCenter,
      const T2 firstRadius,
      const std::array<T3, 2>& secondCenter,
      const T4 secondRadius);

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  std::pair<std::vector<std::array<std::common_type<T1, T2, T3, T4, T5>, 3>>, bool> circleSphereIntersections(
      const std::array<T1, 3>& circleCenter,
      const T2 circleRadius,
      const std::array<T3, 3>& circleNormal,
      const std::array<T4, 3>& sphereCenter,
      const T5 sphereRadius);
}
