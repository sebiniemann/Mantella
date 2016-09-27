#pragma once

/**

*/

  arma::vec vanDerCorputSequence(
      const arma::uword base,
      const arma::uword seed,
      const arma::uword numberOfElements) {
    assert(base > 1 && "vanDerCorputSequence: The base must be greater than 1.");

    if (numberOfElements == 0) {
      return {};
    }

    /* The *n*-th number of a van der Corput sequence can be calculated as
     * follows:
     *
     * infinity
     *   sum    d_i * `base`^(-i)
     *  i = 1
     * 
     * with *d_i* being the *i*-th digit of the *n*-th `base`-adic number.
     */

    if (std::numeric_limits<arma::uword>::max() - numberOfElements < seed) {
      throw std::range_error("vanDerCorputSequence: The seed plus the number of elements must be less than or equal to the largest supported integer.");
    } else if (!isRepresentableAsFloatingPoint(base)) {
      throw std::range_error("vanDerCorputSequence: The base must be representable as a floating point.");
    }

    arma::vec sequence(numberOfElements);
    for (arma::uword n = 0; n < numberOfElements; ++n) {
      double number = 0.0;

      double inverseDigitBase = 1.0 / static_cast<decltype(inverseDigitBase)>(base);
      arma::uword remaining = n + seed;
      while (remaining > 0) {
        number += static_cast<decltype(number)>(remaining % base) * inverseDigitBase;

        inverseDigitBase /= static_cast<decltype(inverseDigitBase)>(base);
        // **Note:** The implicit integer flooring is the wanted behaviour.
        remaining /= base;
      }

      sequence(n) = number;
    }

    return sequence;
  }