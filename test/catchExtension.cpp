#include <catchExtension.hpp>

// C++ standard library
#include <cmath>

void COMPARE_SET(
    const std::vector<std::pair<arma::Col<double>, double>>& actual,
    const std::vector<arma::Col<double>>& expected) {
  CHECK(actual.size() == expected.size());

  bool hasSameParameters = true;
  for (const auto& expectedSample : expected) {
    bool found = false;
    for (const auto& actualSample : actual) {
      if (arma::all(arma::abs(expectedSample - actualSample.first) < 1e-12)) {
        found = true;
        break;
      }
    }

    CHECK(found == true);

    if (!found) {
      hasSameParameters = false;
      break;
    }
  }

  CHECK(hasSameParameters == true);
}

void COMPARE(
    const arma::Col<double>& actual,
    const arma::Col<double>& expected) {
  CHECK(actual.n_elem == expected.n_elem);

  for (arma::uword n = 0; n < expected.n_elem; ++n) {
    if (std::isfinite(expected(n))) {
      CHECK(actual(n) == Approx(expected(n)));
    } else {
      CHECK(std::isinf(actual(n)));
      CHECK(std::signbit(actual(n)) == std::signbit(expected(n)));
    }
  }
}

void COMPARE(
    const arma::Col<arma::uword>& actual,
    const arma::Col<arma::uword>& expected) {
  CHECK(actual.n_elem == expected.n_elem);

  for (arma::uword n = 0; n < expected.n_elem; ++n) {
    if (std::isfinite(expected(n))) {
      CHECK(actual(n) == Approx(expected(n)));
    } else {
      CHECK(std::isinf(actual(n)));
      CHECK(std::signbit(actual(n)) == std::signbit(expected(n)));
    }
  }
}

void COMPARE(
    const arma::Mat<double>& actual,
    const arma::Mat<double>& expected) {
  CHECK(actual.n_rows == expected.n_rows);
  CHECK(actual.n_cols == expected.n_cols);

  for (arma::uword n = 0; n < expected.n_cols; ++n) {
    if (std::isfinite(expected(n))) {
      CHECK(actual(n) == Approx(expected(n)));
    } else {
      CHECK(std::isinf(actual(n)));
      CHECK(std::signbit(actual(n)) == std::signbit(expected(n)));
    }
  }
}
