#include "mantella_bits/randomNumberGenerator.hpp"
#include "mantella_bits/config.hpp" // IWYU pragma: keep

namespace mant {
  decltype(Rng::seed_) Rng::seed_ = 12345;
  decltype(Rng::generator_) Rng::generator_;

  std::mt19937_64& Rng::getGenerator() {
    return generator_;
  }

  void Rng::setSeed(
      const arma::arma_rng::seed_type seed) {
    seed_ = seed;

    // Seeding C++'s and Armadillo's generator.
    generator_.seed(seed_);
    arma::arma_rng::set_seed(seed_);
  }

  void Rng::setRandomSeed() {
    // Initialise Armadillo's generator with any random seed.
    // This seed will be overwritten later on, no need to store it.
    arma::arma_rng::set_seed_random();
#if defined(SUPPORT_MPI)
    // For programs using MPI, each node needs to seeded differently in order to avoid a degeneration of the random number generator, due to synchronisation.
    // Basically, we generated as much random numbers as there are nodes and seed the i-th node by the i-th generated random number.
    int nodeRank;
    int numberOfNodes;
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfNodes);

    setSeed(arma::randi<arma::Col<arma::arma_rng::seed_type>>(static_cast<arma::uword>(numberOfNodes))(static_cast<arma::uword>(nodeRank)));
#else
    // Without MPI, using the first random number generated by Armadillo's generator as seed is sufficient.
    setSeed(arma::randi<arma::Col<arma::arma_rng::seed_type>>(1)(0));
#endif
  }

  arma::arma_rng::seed_type Rng::getSeed() {
    return seed_;
  }
}
