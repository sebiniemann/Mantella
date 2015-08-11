#include <mantella_bits/helper/rng.hpp>

// Armadillo
#include <armadillo>

namespace mant {
  std::mt19937_64& Rng::getGenerator() {
    static std::mt19937_64 generator;
    return generator;
  }

  void Rng::setSeed(
      const std::mt19937_64::result_type seed) {
   seed_() = seed;

    getGenerator().seed(seed_());
    arma::arma_rng::set_seed(seed_());

  }

  void Rng::setRandomSeed() {
    arma::arma_rng::set_seed_random();
#if defined(MANTELLA_USE_MPI)
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    setSeed(arma::randi<arma::Col<arma::uword>>(1)(0) + rank * arma::randi<arma::Col<arma::uword>>(1)(0));
#else
    setSeed(arma::randi<arma::Col<arma::uword>>(1)(0));
#endif
  }

  std::mt19937_64::result_type Rng::getSeed() {
    return seed_();
  }

  std::mt19937_64::result_type& Rng::seed_() {
    static std::mt19937_64::result_type seed;
    return seed;
  }
}
