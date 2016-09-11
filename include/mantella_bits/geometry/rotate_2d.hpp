#pragma once

template <typename T>
std::array<T, 2> rotate_2d(
    const typename std::array<T, 2>::const_iterator vector,
    const T angle);
    
//
// Implementation
//

template <typename T>
std::array<T, 4> rotate_2d(
    const typename std::array<T, 2>::const_iterator vector,
    const T angle) {
  assert(std::isfinite(angle) && "rotation_matrix_2d: The angle must be finite.");

  const double vector_0 = *vector;
  const double vector_1 = *(++vector);
  
  const double sin_angle = std::sin(angle);
  const double cos_angle = std::cos(angle);
  
  /* Solution of
   *
   * / x_0 \  =  / vector_0 \  *  / cos(angle), -sin(angle) \
   * \ x_1 /     \ vector_1 /     \ sin(angle),  cos(angle) /
   */
  return {vector_0 * cos_angle - vector_1 * sin_angle, vector_0 * sin_angle + vector_1 * cos_angle};
}