#pragma once

// C++ standard library
#include <cmath>
#include <string>

// Armadillo
#include <armadillo>

namespace mant {
  template <typename T>
  arma::Col<T> range(
      const T start,
      const T end,
      const T stepSize);

  template <typename T>
  arma::Col<T> range(
      const T start,
      const T end);

  class Hash {
   public:
    arma::uword operator()(
        const arma::Col<double>& key) const;
  };

  class IsEqual {
   public:
    bool operator()(
        const arma::Col<double>& firstKey,
        const arma::Col<double>& secondKey) const;
  };
}

//
// Template implementation
//

// Mantella
// Resolves a circular dependency between this header and assert.hpp
namespace mant {
  void verify(
      const bool expression,
      const std::string& errorMessage);
}

namespace mant {
  template <typename T>
  arma::Col<T> range(
      const T start,
      const T end,
      const T stepSize) {
    static_assert(arma::is_supported_elem_type<T>::value, "range: The template parameter must be compatible with Armadillo C++.");

    verify(stepSize > 0, "range: The step size must be strict greater than 0.");
    if (start <= end) {
      // Adds one to the number of elements, to include the starting point.
      // In case *T* is not an integer, we need to cast the number of steps to an integer type.
      return arma::linspace<arma::Col<T>>(start, end, static_cast<arma::uword>(std::floor((end - start) / stepSize + 1)));
    } else {
      // Adds one to the number of elements, to include the starting point.
      // In case *T* is not an integer, we need to cast the number of steps to an integer type.
      arma::Col<T> range(static_cast<arma::uword>(std::floor((start - end) / stepSize + 1)));

      for (arma::uword n = 0; n < range.n_elem; ++n) {
        // Calculates the next element from scratch (instead of increasing it step by step) to reduce rounding errors.
        range(n) = start - stepSize * n;
      }

      return range;
    }
  }

  template <typename T>
  arma::Col<T> range(
      const T start,
      const T end) {
    static_assert(arma::is_supported_elem_type<T>::value, "range: The template parameter must be compatible with Armadillo C++.");

    return range<T>(start, end, 1);
  }
}
