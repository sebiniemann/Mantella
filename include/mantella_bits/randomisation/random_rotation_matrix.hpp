#pragma once

/**

*/
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
  for (std::size_t n = 0; n < N*N; ++n) {
    random_rotation_matrix[n] = std::normal_distribution<T>()(random_number_generators()[thread_number()]);
  }
  
  
  // Computing the matrices *Q* and *R* of a QR decomposition using LAPACK is based on two steaps.
  // 1. At first, the upper triangular matrix *R* is calculated using `[s,d]geqrf`, while *Q* we given as a product of `N` elementary reflectors.
  // 2. Next, the orthogonal matrix *Q* is calculated, based on the previous `N` elementary reflectors, using `[s,d]orgqr`.
  // @see https://software.intel.com/en-us/node/468944 or http://www.netlib.no/netlib/lapack/double/dgeqrf.f (documentation on [s,d]geqrf)
  // @see https://software.intel.com/en-us/node/468956 or http://www.netlib.no/netlib/lapack/double/dorgqr.f (documentation on [s,d]orgqr)
  
  // Computes the upper triangular matrix *R*.
  // Until `[s,d]orgqr` is called, *Q* will contain the upper triangular matrix *R*, while the elements below the diagonal contain part of the `N` elementary reflectors.
  std::array<T, N*N> q;
  std::copy_n(iterator, N*N, q.begin());
  
  std::array<T, N> tau;
  std::array<T, N> work;
  int info;

  dgeqrf_(N, N, q.data(), N, tau.data(), work.data(), N, &info);
  
  if (info != 0) {
    return {{}, false};
  }
  
  // Retrieves the upper triangular matrix *R* from *Q*.
  std::array<T, N*N> r;
  for (std::size_t n = 0; n < N; ++n) {
    for (std::size_t k = 0; k < N; ++k) {
      if (n <= k) {
        r[n*N + k] = q[n*N + k];
      } else {
        r[n*N + k] = 0.0;
      }
    }
  }
  
  // Computes the orthogonal matrix *R*.
  dorgqr_(N, N, N, q.data(), N, tau.data(), work.data(), N, &info);

  if (info != 0) {
    return {{}, false};
  } else {
    return {{r, q}, true};
  }
  
  
  if (decomposition.second) {
    random_rotation_matrix = decomposition.first.first;
    for (std::size_t n = 0; n < N; ++n) {
      if (decomposition.first.second[n*N + n] < 0.0) {
        random_rotation_matrix[n*N + n] *= -1.0;
      }
    }
  } else {
    // In very rare cases, the QR decomposition fails due to the limited numerical precision. In these rare cases, we simply return an identity matrix.
    random_rotation_matrix.fill(0.0);
    for (std::size_t n = 0; n < N; ++n) {
      random_rotation_matrix[n*N + n] = 1.0;
    }
  }
    
  return random_rotation_matrix;
}