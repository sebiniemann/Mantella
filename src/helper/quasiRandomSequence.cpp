#include <mantella_bits/helper/quasiRandomSequence.hpp>

// Mantella
#include <mantella_bits/helper/assert.hpp>

namespace mant {
  arma::Mat<double> getHaltonSequence(
      const arma::Col<arma::uword>& base,
      const arma::Col<arma::uword>& seed,
      const arma::uword numberOfColumms) {
    verify(base.n_elem == seed.n_elem,
        "The number of dimensions of the base parameter must match the number of dimensions of the seed parameter.");

    arma::Mat<double> sequence(seed.n_elem, numberOfColumms);

    for (arma::uword n = 0; n < seed.n_elem; ++n) {
      sequence.row(n) = getVanDerCorputSequence(base(n), seed(n), numberOfColumms).t();
    }

    return sequence;
  }

  arma::Col<double> getVanDerCorputSequence(
      const arma::uword base,
      const arma::uword seed,
      const arma::uword numberOfColumms) {
    arma::Col<double> sequence(numberOfColumms);

    for (arma::uword n = 0; n < numberOfColumms; ++n) {
      double inverse = 1.0 / static_cast<double>(base);
      double number = 0.0;

      arma::uword remaining = seed + n;
      while (remaining > 0) {
        arma::uword digit = remaining % base;
        number += static_cast<double>(digit) * inverse;
        inverse /= static_cast<double>(base);
        // This will already be floored, as both are integer types.
        remaining /= base;
      }

      sequence(n) = number;
    }

    return sequence;
  }
}
