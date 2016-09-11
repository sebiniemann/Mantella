#pragma once

template <typename T, std::size_t N>
std::pair<std::array<T, N*N>, std::array<T, N*N>> qr_decomposition(
    const std::array<T, N*N>& matrix);

//
// Implementation
//

template <typename T, std::size_t N>
std::pair<std::array<T, N*N>, std::array<T, N*N>> qr_decomposition(
    const typename std::array<T, N*N>::const_iterator iterator) {
  static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be a `float` or `double`.");
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

  if (std::is_same<T, float>::value) {
    LAPACKE_sgeqrf(N, N, q.data(), N, tau.data(), work.data(), N, &info);
  } else {
    LAPACKE_dgeqrf_(N, N, q.data(), N, tau.data(), work.data(), N, &info);
  }
  
  if (info != 0) {
    return {{}, false};
  }
  
  // Retrieves the upper triangular matrix *R* from *Q*.
  std::array<T, N*N> r;
  for (decltype(N) n = 0; n < N; ++n) {
    for (decltype(N) k = 0; k < N; ++k) {
      if (n <= k) {
        r[n*N + k] = q[n*N + k];
      } else {
        r[n*N + k] = 0.0;
      }
    }
  }
  
  // Computes the orthogonal matrix *R*.
  if (std::is_same<T, float>::value) {
    LAPACKE_sorgqr(N, N, N, q.data(), N, tau.data(), work.data(), N, &info);
  } else {
    LAPACKE_dorgqr(N, N, N, q.data(), N, tau.data(), work.data(), N, &info);
  }

  if (info != 0) {
    return {{}, false};
  } else {
    return {{r, q}, true};
  }
}