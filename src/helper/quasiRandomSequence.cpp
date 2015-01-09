#include <mantella_bits/helper/quasiRandomSequence.hpp>

// C++ Standard Library
#include <cstdlib>

namespace mant {
  arma::Mat<double> getHaltonSequence(
      const arma::Col<unsigned int>& seed,
      const arma::Col<unsigned int>& base,
      const unsigned int& numberOfColumms) noexcept {
    arma::Mat<double> sequence(seed.n_elem, numberOfColumms);

    for (std::size_t n = 0; n < seed.n_elem; ++n) {
      sequence.col(n) = getVanDerCorputSequence(seed.at(n), base.at(n), numberOfColumms);
    }

    return sequence;
  }

  arma::Col<double> getVanDerCorputSequence(
      const unsigned int& seed,
      const unsigned int& base,
      const unsigned int& numberOfColumms) noexcept {
    arma::Col<double> sequence(numberOfColumms);

    for (std::size_t n = 0; n < numberOfColumms; ++n) {
      double inverse = 1.0 / static_cast<double>(base);
      double number = 0.0;

      unsigned int remaining = seed + n;
      while (remaining > 0) {
        unsigned int digit = remaining % base;
        number = number + static_cast<double>(digit) * inverse;
        inverse = inverse / static_cast<double>(base);
        remaining = std::floor(remaining / base);
      }

      sequence.at(n) = number;
    }

    return sequence;
  }
}
