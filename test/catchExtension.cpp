#include "catchExtension.hpp"

// C++ standard library
#include <algorithm>
#include <cmath>
#include <random>

// Mantella
#include <mantella>

std::string rootTestDataDirectory("");
int nodeRank(0);

arma::uword getDiscreteRandomNumber() {
  return std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
}

arma::uword getDifferentDiscreteRandomNumber(
    const arma::uword discreteRandomNumber) {
  arma::uword randomNumber = std::uniform_int_distribution<arma::uword>(1, 9)(mant::Rng::getGenerator());
  if (randomNumber >= discreteRandomNumber) {
    ++randomNumber;
  }
  
  return randomNumber;
}

double getContinuousRandomNumber() {
  return std::uniform_real_distribution<double>(-100.0, 100.0)(mant::Rng::getGenerator());
}

arma::Col<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfElements) {
  return arma::randi<arma::Col<arma::uword>>(numberOfElements, arma::distr_param(1, 10));
}

arma::Col<double> getContinuousRandomNumbers(
    const arma::uword numberOfDimensions) {
  return arma::randu<arma::Col<double>>(numberOfDimensions) * 200.0 - 100.0;
}

arma::Mat<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns) {
  return arma::randi<arma::Mat<arma::uword>>(numberOfRows, numberOfColumns, arma::distr_param(1, 10));
}

arma::Mat<double> getContinuousRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns) {
  return arma::randu<arma::Mat<double>>(numberOfRows, numberOfColumns) * 200.0 - 100.0;
}

arma::Mat<double> SYNCRONISED(
    const arma::Mat<double>& data) {
  arma::Mat<double> syncronisedData = data;
  MPI_Bcast(syncronisedData.memptr(), static_cast<int>(syncronisedData.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return syncronisedData;
}

arma::Mat<arma::uword> SYNCRONISED(
    const arma::Mat<arma::uword>& data) {
  arma::Mat<unsigned int> syncronisedData = arma::conv_to<arma::Mat<unsigned int>>::from(data);
  MPI_Bcast(syncronisedData.memptr(), static_cast<int>(syncronisedData.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  return arma::conv_to<arma::Mat<arma::uword>>::from(syncronisedData);
}

arma::Col<double> SYNCRONISED(
    const arma::Col<double>& data) {
  arma::Col<double> syncronisedData = data;
  MPI_Bcast(syncronisedData.memptr(), static_cast<int>(syncronisedData.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return syncronisedData;
}

arma::Col<arma::uword> SYNCRONISED(
    const arma::Col<arma::uword>& data) {
  arma::Col<unsigned int> syncronisedData = arma::conv_to<arma::Col<unsigned int>>::from(data);
  MPI_Bcast(syncronisedData.memptr(), static_cast<int>(syncronisedData.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  return arma::conv_to<arma::Col<arma::uword>>::from(syncronisedData);
}

arma::Row<double> SYNCRONISED(
    const arma::Row<double>& data) {
  arma::Row<double> syncronisedData = data;
  MPI_Bcast(syncronisedData.memptr(), static_cast<int>(syncronisedData.n_elem), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return syncronisedData;
}

arma::Row<arma::uword> SYNCRONISED(
    const arma::Row<arma::uword>& data) {
  arma::Row<unsigned int> syncronisedData = arma::conv_to<arma::Row<unsigned int>>::from(data);
  MPI_Bcast(syncronisedData.memptr(), static_cast<int>(syncronisedData.n_elem), MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  return arma::conv_to<arma::Row<arma::uword>>::from(syncronisedData);
}

double SYNCRONISED(
    const double data) {
  double syncronisedData = data;
  MPI_Bcast(&syncronisedData, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  return syncronisedData;
}

arma::uword SYNCRONISED(
    const arma::uword data) {
  unsigned int syncronisedData = static_cast<unsigned int>(data);
  MPI_Bcast(&syncronisedData, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
  return static_cast<arma::uword>(syncronisedData);
}

void HAS_SAME_SAMPLES(
    const std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual>& actualSamples,
    const std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual>& expectedSamples) {
  CHECK(actualSamples.size() == expectedSamples.size());
  for (const auto& expectedSample  : expectedSamples) {
    const auto actualSample = actualSamples.find(expectedSample.first);
    
    // Found *expectedSample* within *actualSamples*.
    CHECK(actualSample != actualSamples.cend());
    CHECK(actualSample->second == Approx(expectedSample.second));
  }
}

void HAS_SAME_PARAMETERS(
    const std::vector<arma::Col<arma::uword>>& actualParameters,
    const std::vector<arma::Col<arma::uword>>& expectedParameters) {
  CHECK(actualParameters.size() == expectedParameters.size());

  for (const auto& expectedParameter : expectedParameters) {
    CAPTURE(expectedParameter);
    
    bool found = false;
    for (const auto& actualParameter : actualParameters) {
      if (expectedParameter.n_elem != actualParameter.n_elem) {
        continue;
      }
      
      if (arma::all(expectedParameter == actualParameter)) {
        found = true;
        break;
      }
    }

    CHECK(found == true);
  }
}

void HAS_SAME_PARAMETERS(
    const std::vector<std::pair<arma::Col<double>, double>>& actualSamples,
    const std::vector<arma::Col<double>>& expectedParameters) {
  CHECK(actualSamples.size() == expectedParameters.size());

  for (const auto& expectedParameter : expectedParameters) {
    CAPTURE(expectedParameter);
    
    bool found = false;
    for (const auto& actualSample : actualSamples) {
      if (expectedParameter.n_elem != actualSample.first.n_elem) {
        continue;
      }
    
      // Both parameters are considered to be equal, if there relative error is within 1e-12.
      if (arma::all(arma::abs(expectedParameter - actualSample.first) < 1e-12 * arma::max(arma::ones(arma::size(expectedParameter)), arma::abs(expectedParameter)))) {
        found = true;
        break;
      }
    }

    CHECK(found == true);
  }
}

void HAS_SAME_ELEMENTS(
    const arma::Col<double>& actualElements,
    const arma::Col<double>& expectedElements) {
  CHECK(actualElements.n_elem == expectedElements.n_elem);

  CAPTURE(actualElements);
  CAPTURE(expectedElements);
    
  for (const auto& expectedElement : expectedElements) {
    bool found = false;
    for (const auto& actualElement : actualElements) {
      // Both parameters are considered to be equal, if there relative error is within 1e-12.
      if (std::abs(expectedElement - actualElement) < 1e-12 * std::max(1.0, std::abs(expectedElement))) {
        found = true;
        break;
      }
    }

    CHECK(found == true);
  }
}

void IS_EQUAL(
    const arma::Cube<double>& actual,
    const arma::Cube<double>& expected) {
  CHECK(actual.n_rows == expected.n_rows);
  CHECK(actual.n_cols == expected.n_cols);
  CHECK(actual.n_slices == expected.n_slices);

  for (arma::uword n = 0; n < expected.n_elem; ++n) {
    CHECK(std::isfinite(actual(n)));
    CHECK(actual(n) == Approx(expected(n)));
  }
}

void IS_EQUAL(
    const arma::Mat<double>& actual,
    const arma::Mat<double>& expected) {
  CHECK(actual.n_rows == expected.n_rows);
  CHECK(actual.n_cols == expected.n_cols);

  for (arma::uword n = 0; n < expected.n_elem; ++n) {
    CHECK(std::isfinite(actual(n)));
    CHECK(actual(n) == Approx(expected(n)));
  }
}

void IS_EQUAL(
    const arma::Col<double>& actual,
    const arma::Col<double>& expected) {
  CHECK(actual.n_elem == expected.n_elem);

  for (arma::uword n = 0; n < expected.n_elem; ++n) {
    CHECK(std::isfinite(actual(n)));
    CHECK(actual(n) == Approx(expected(n)));
  }
}

void IS_EQUAL(
    const arma::Col<arma::uword>& actual,
    const arma::Col<arma::uword>& expected) {
  CHECK(actual.n_elem == expected.n_elem);

  for (arma::uword n = 0; n < expected.n_elem; ++n) {
    CHECK(actual(n) == expected(n));
  }
}

void IS_UNIFORM(
    const arma::Col<double>& actualData,
    const double expectedLowerBound,
    const double expectedUpperBound) {
  CHECK(arma::all(expectedLowerBound <= actualData));
  CHECK(arma::all(actualData <= expectedUpperBound));
    
  const arma::uword numberOfBins = 10;
  CAPTURE(numberOfBins);
  const double binDistance = (expectedUpperBound - expectedLowerBound) / numberOfBins;
  CAPTURE(binDistance);
  const arma::Col<double>& bins = arma::linspace<arma::Col<double>>(expectedLowerBound, expectedUpperBound - binDistance, numberOfBins) + binDistance / 2;
  CAPTURE(bins);
  
  const arma::Col<arma::uword>& histogram = arma::hist(actualData, bins);
  CAPTURE(histogram);
  
  // The histogram is considered to be uniform, if there absolute error is lower than 5%.
  CHECK((histogram.max() - histogram.min()) < static_cast<arma::uword>(0.05 * static_cast<double>(actualData.n_elem)));
}

void IS_UNIFORM(
    const arma::Col<arma::uword>& actualData,
    const arma::uword expectedLowerBound,
    const arma::uword expectedUpperBound) {
  CHECK(arma::all(expectedLowerBound <= actualData));
  CHECK(arma::all(actualData <= expectedUpperBound));
  
  const arma::Col<arma::uword>& bins = arma::linspace<arma::Col<arma::uword>>(expectedLowerBound, expectedUpperBound, expectedUpperBound - expectedLowerBound + 1);
  CAPTURE(bins);
  
  const arma::Col<arma::uword>& histogram = arma::hist(actualData, bins);
  CAPTURE(histogram);
  
  // The histogram is considered to be uniform, if there absolute error is lower than 5%.
  CHECK((histogram.max() - histogram.min()) < static_cast<arma::uword>(0.05 * static_cast<double>(actualData.n_elem)));
}
