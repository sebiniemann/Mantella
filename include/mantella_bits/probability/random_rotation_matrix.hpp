#pragma once

template <typename T, std::size_t N>
std::array<T, N*N> random_rotation_matrix();

//
// Implementation
//

// @see Francesco Mezzadri (2007). How to Generate Random Matrices from the Classical Compact Groups. Notices of the AMS, 54(5), pp. 592-604.
template <typename T, std::size_t N>
std::array<T, N*N> random_rotation_matrix() {
  static_assert(N > 1, "The number of dimensions must be greater than 1.");

  std::array<T, N*N> random_rotation_matrix;
  // Fills the memory first with intermediate values required in the computation and replaces it later on with the actual rotation matrix, to reduce the number of temporary memory allocations.
  for (decltype(N*N) n = 0; n < N*N; ++n) {
    random_rotation_matrix[n] = std::normal_distribution<T>()(random_number_generators()[thread_number()]);
  }
  const auto decomposition = qr_decomposition(random_rotation_matrix);
  
  if (decomposition.second) {
    random_rotation_matrix = decomposition.first.first;
    for (decltype(N) n = 0; n < N; ++n) {
      if (decomposition.first.second[n*N + n] < 0.0) {
        random_rotation_matrix[n*N + n] *= -1.0;
      }
    }
  } else {
    // In very rare cases, the QR decomposition fails due to the limited numerical precision. In these rare cases, we simply return an identity matrix.
    random_rotation_matrix.fill(0.0);
    for (decltype(N) n = 0; n < N; ++n) {
      random_rotation_matrix[n*N + n] = 1.0;
    }
  }
    
  return random_rotation_matrix;
}