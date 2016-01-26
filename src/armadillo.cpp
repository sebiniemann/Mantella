#include "mantella_bits/armadillo.hpp"

// C++ standard library
#include <functional>

// Mantella
#include "mantella_bits/assert.hpp"

namespace mant {
  arma::Col<arma::uword> range(
      const arma::uword start,
      const arma::uword end,
      const arma::uword stepSize) {
    verify(stepSize > 0, "range: The step size must be strict greater than 0.");

    if (start == end) {
      return {start};
    } else if (start < end) {
      // Adds one to the number of elements, to include the starting point.
      return arma::linspace<arma::Col<arma::uword>>(start, end, (end - start) / stepSize + 1);
    } else {
      // Adds one to the number of elements, to include the starting point.
      arma::Col<arma::uword> range((start - end) / stepSize + 1);

      for (arma::uword n = 0; n < range.n_elem; ++n) {
        // Calculates the next element from scratch (instead of increasing it step by step), to reduce rounding errors.
        range(n) = start - stepSize * n;
      }

      return range;
    }
  }

  arma::Col<arma::uword> range(
      const arma::uword start,
      const arma::uword end) {
    return range(start, end, 1);
  }

  arma::uword Hash::operator()(
      const arma::Col<double>& key) const {
    // Starts with the hash of the first value ...
    arma::uword hashedKey = static_cast<arma::uword>(std::hash<double>()(key(0)));

    // ... and adds the hash value of all following values to it.
    // Note: This is adapted from the Boost library (boost::hash_combine), including the magic numbers.
    for (const auto& value : key) {
      hashedKey ^= static_cast<arma::uword>(std::hash<double>()(value)) + 0x9e3779b9 + (hashedKey << 6) + (hashedKey >> 2);
    }

    return hashedKey;
  }

  bool IsEqual::operator()(
      const arma::Col<double>& firstKey,
      const arma::Col<double>& secondKey) const {
    if (firstKey.n_elem != secondKey.n_elem) {
      return false;
    }

    return arma::all(firstKey == secondKey);
  }
}
