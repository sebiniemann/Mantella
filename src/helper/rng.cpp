#include <mantella_bits/helper/rng.hpp>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella_bits/config.hpp>

namespace mant {
  std::mt19937_64& Rng::getGenerator() {
    static std::mt19937_64 generator;
    return generator;
  }

  void Rng::setSeed(
      const arma::arma_rng::seed_type seed) {
    seed_() = seed;

    getGenerator().seed(seed_());
    arma::arma_rng::set_seed(seed_());
  }

  void Rng::setRandomSeed() {
    arma::arma_rng::set_seed_random();
#if defined(SUPPORT_MPI)
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    setSeed(arma::randi<arma::Col<arma::arma_rng::seed_type>>(1)(0) + static_cast<unsigned int>(rank) * arma::randi<arma::Col<arma::arma_rng::seed_type>>(1)(0));
#else
    setSeed(arma::randi<arma::Col<arma::arma_rng::seed_type>>(1)(0));
#endif
  }

  arma::arma_rng::seed_type Rng::getSeed() {
    return seed_();
  }

  arma::arma_rng::seed_type& Rng::seed_() {
    static arma::arma_rng::seed_type seed;
    return seed;
  }
}
