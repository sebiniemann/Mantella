#include "mantella_bits/randomNumberGenerator.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

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
  decltype(Rng::generator_) Rng::generator_;

  void Rng::setSeed(
      const std::random_device::result_type seed) {
    // Seeds C++'s and Armadillo's random number generator.
    generator_.seed(seed);
    arma::arma_rng::set_seed(seed);
  }

  std::random_device::result_type Rng::setRandomSeed() {
    int nodeRank;
#if defined(SUPPORT_MPI)
    // With MPI, each node needs to be seeded differently in order to avoid degenerating the random number generator, due to synchronisation.
    // This is done by generating as much random numbers as there are nodes and seeding the `n`-th node by the `n`-th generated random number.
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank);
    if (nodeRank < 0) {
      throw std::runtime_error("setRandomSeed: Please check your MPI installation, as we got a negative node rank.");
    }
#else
    nodeRank = 0;
#endif

    std::random_device randomDevice;
    std::random_device::result_type seed = randomDevice();
    // `arma::uword` should always be greater than `int`.
    for (arma::uword n = 1; n <= static_cast<decltype(n)>(nodeRank); ++n) {
      seed = randomDevice();
    }

    setSeed(seed);

    return seed;
  }
}
