#pragma once

template <typename T>
std::array<T, 3> rotate_3d(
    const typename std::array<T, 3>::const_iterator vector,
    const T x_angle,
    const T y_angle,
    const T z_angle);
    
//
// Implementation
//

template <typename T>
std::array<T, 3> rotate_3d(
    const typename std::array<T, 3>::const_iterator vector,
    const T x_angle,
    const T y_angle,
    const T z_angle) {
  assert(std::isfinite(x_angle) && "rotation_matrix_3d: The roll angle must be finite.");
  assert(std::isfinite(y_angle) && "rotation_matrix_3d: The pitch angle must be finite.");
  assert(std::isfinite(z_angle) && "rotation_matrix_3d: The yaw angle must be finite.");

  const double vector_0 = *vector;
  const double vector_1 = *(++vector);
  const double vector_2 = *(++vector);
  
  const double sin_x_angle = std::sin(x_angle);
  const double cos_x_angle = std::cos(x_angle);
  const double sin_y_angle = std::sin(y_angle);
  const double cos_y_angle = std::cos(y_angle);
  const double sin_z_angle = std::sin(z_angle);
  const double cos_z_angle = std::cos(z_angle);
  
  /* Solution of
   *
   * / x_0 \     / vector_0 \     / 1,            0,             0 \     / cos(y_angle), 0, -sin(y_angle) \     / cos(z_angle), -sin(z_angle), 0 \
   * | x_1 |  =  | vector_1 |  *  | 0, cos(x_angle), -sin(x_angle) |  *  |            0, 1,             0 |  *  | sin(z_angle),  cos(z_angle), 0 |
   * \ x_2 /     \ vector_2 /     \ 0, sin(x_angle),  cos(x_angle) /     \ sin(y_angle), 0,  cos(y_angle) /     \            0,             0, 1 /
   */
  return {vector_0 * cos_y_angle * cos_z_angle + vector_1 * sin_x_angle * sin_y_angle * cos_z_angle + vector_2 * cos_x_angle * sin_y_angle * cos_z_angle - vector_1 * cos_x_angle * sin_z_angle + vector_2 * sin_x_angle * sin_z_angle, vector_0 * cos_y_angle * sin_z_angle + vector_1 * sin_x_angle * sin_y_angle * sin_z_angle + vector_2 * cos_x_angle * sin_y_angle * sin_z_angle + vector_1 * cos_x_angle * cos_z_angle - vector_2 * sin_x_angle * cos_z_angle, vector_0 * -sin_y_angle + vector_1 * sin_x_angle * cos_y_angle + vector_2 * cos_x_angle * cos_y_angle};
}