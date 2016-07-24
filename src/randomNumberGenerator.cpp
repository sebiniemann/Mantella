#include "mantella_bits/randomNumberGenerator.hpp"

// C++ standard library
#if defined(SUPPORT_MPI)
#include <stdexcept> // IWYU pragma: keep
#endif

// MPI
#if defined(SUPPORT_MPI)
#include <mpi.h> // IWYU pragma: keep
#endif

// Armadillo
#include <armadillo>

namespace mant {
  std::random_device::result_type Rng::setRandomSeed() {
    std::random_device randomDevice;
    std::random_device::result_type seed = randomDevice();

    // Provides a different seed for each MPI node.
    for (arma::uword n = 1; n <= static_cast<decltype(n)>(nodeRank()); ++n) {
      seed = randomDevice();
    }

    setSeed(seed);

    return seed;
  }
}
