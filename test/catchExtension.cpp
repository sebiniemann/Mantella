#include <catchExtension.hpp>

// C++ standard library
#include <cmath>
#include <algorithm>

// Mantella
#include <mantella>

std::string testDirectory("");

arma::uword getDiscreteRandomNumber() {
  return std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
}

arma::uword getDifferentDiscreteRandomNumber(
    const arma::uword discreteRandomNumber) {
  arma::uword offset = std::uniform_int_distribution<arma::uword>(1, 5)(mant::Rng::getGenerator());
  const bool isNegativeOffset = std::bernoulli_distribution(0.5)(mant::Rng::getGenerator());
  
  if (isNegativeOffset && discreteRandomNumber > offset) {
    return discreteRandomNumber - offset;
  } else {
    return discreteRandomNumber + offset;
  }
}

double getContinuousRandomNumber() {
  return std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
}

arma::Col<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfElements) {
  return arma::randi<arma::Col<arma::uword>>(numberOfElements, arma::distr_param(0, 10));
}

arma::Col<double> getContinuousRandomNumbers(
    const arma::uword numberOfDimensions) {
  return arma::randu<arma::Col<double>>(numberOfDimensions) * 200 - 100;
}

arma::Mat<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns) {
  return arma::randi<arma::Mat<arma::uword>>(numberOfRows, numberOfColumns, arma::distr_param(0, 10));
}

arma::Mat<double> getContinuousRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns) {
  return arma::randu<arma::Mat<double>>(numberOfRows, numberOfColumns) * 200 - 100;
}

void HAS_SAME_PARAMETERS(
    const std::vector<arma::Col<arma::uword>>& actualParameters,
    const std::vector<arma::Col<arma::uword>>& expectedParameters) {
  CHECK((actualParameters.size() == expectedParameters.size()));

  for (const auto& expectedParameter : expectedParameters) {
    CAPTURE(expectedParameter);
    
    bool found = false;
    for (const auto& actualParameter : actualParameters) {
      if (arma::all(expectedParameter == actualParameter)) {
        found = true;
        break;
      }
    }

    CHECK((found == true));
  }
}

void HAS_SAME_PARAMETERS(
    const std::vector<std::pair<arma::Col<double>, double>>& samples,
    const std::vector<arma::Col<double>>& parameters) {
  CHECK(samples.size() == parameters.size());

  for (const auto& parameter : parameters) {
    CAPTURE(parameter);
    
    bool found = false;
    for (const auto& sample : samples) {
      if (arma::all(arma::abs(parameter - sample.first) < 1e-12)) {
        found = true;
        break;
      }
    }

    CHECK(found == true);
  }
}

void IS_EQUAL(
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

void IS_EQUAL(
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

void IS_EQUAL(
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

void IS_UNIFORM(
    const arma::Col<double>& data,
    const double lowerBound,
    const double upperBound) {
  const arma::uword numberOfBins = 10;
  CAPTURE(numberOfBins);
  const double binDistance = (upperBound - lowerBound) / numberOfBins;
  CAPTURE(binDistance);
  const arma::Col<double>& bins = arma::linspace<arma::Col<double>>(lowerBound, upperBound - binDistance, numberOfBins) + binDistance / 2;
  CAPTURE(bins);
  
  const arma::Col<arma::uword>& histogram = arma::hist(data, bins);
  CAPTURE(histogram);
  
  CHECK((histogram.max() - histogram.min()) < static_cast<arma::uword>(0.05 * static_cast<double>(data.n_elem)));
}

void IS_UNIFORM(
    const arma::Col<arma::uword>& data,
    const arma::uword lowerBound,
    const arma::uword upperBound) {
  const arma::Col<arma::uword>& bins = arma::linspace<arma::Col<arma::uword>>(lowerBound, upperBound, upperBound - lowerBound + 1);
  CAPTURE(bins);
  
  const arma::Col<arma::uword>& histogram = arma::hist(data, bins);
  CAPTURE(histogram);
  
  CHECK((histogram.max() - histogram.min()) < static_cast<arma::uword>(0.05 * static_cast<double>(data.n_elem)));
}
