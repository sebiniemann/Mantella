#include "catchHelper.hpp"

// C++ standard library
#include <array>

int nodeRank(0);
int numberOfNodes(1);

namespace std {
  ostream& operator<<(
      ostream& outputStream,
      const unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>& samples) {
    for (const auto& sample : samples) {
      outputStream << "(" << accumulate(sample.first.begin() + 1, sample.first.end(), to_string(sample.first(0)), [](const string& concatenatedElements, const double nextElement) { return concatenatedElements + ", " + to_string(nextElement); }) << ") => " << sample.second << endl;
    }

    return outputStream;
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::vec>& parameters) {
    for (const auto& parameter : parameters) {
      outputStream << "(" << accumulate(parameter.begin() + 1, parameter.end(), to_string(parameter(0)), [](const string& concatenatedElements, const double nextValue) { return concatenatedElements + ", " + to_string(nextValue); }) << ")" << endl;
    }

    return outputStream;
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::vec::fixed<2>>& parameters) {
    return outputStream << vector<arma::vec>(parameters.cbegin(), parameters.cend());
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::vec::fixed<3>>& parameters) {
    return outputStream << vector<arma::vec>(parameters.cbegin(), parameters.cend());
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<pair<arma::vec, double>>& samples) {
    for (const auto& sample : samples) {
      outputStream << "(" << accumulate(sample.first.begin() + 1, sample.first.end(), to_string(sample.first(0)), [](const string& concatenatedElements, const double nextElement) { return concatenatedElements + ", " + to_string(nextElement); }) << ") => " << sample.second << endl;
    }

    return outputStream;
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::uvec>& elements) {
    for (const auto& element : elements) {
      outputStream << "(" << accumulate(element.begin() + 1, element.end(), to_string(element(0)), [](const string& concatenatedElements, const arma::uword nextValue) { return concatenatedElements + ", " + to_string(nextValue); }) << ")" << endl;
    }

    return outputStream;
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<pair<arma::uvec, arma::uvec>>& pairs) {
    const auto elementsConcatenatingFunction =
        [](
            const string& concatenatedElements,
            const arma::uword nextValue) {
          return concatenatedElements + ", " + to_string(nextValue);
        };

    for (const auto& pair : pairs) {
      outputStream << "(" << accumulate(pair.first.begin() + 1, pair.first.end(), to_string(pair.first(0)), elementsConcatenatingFunction) << "), (" << accumulate(pair.second.begin() + 1, pair.second.end(), to_string(pair.second(0)), elementsConcatenatingFunction) << ")" << endl;
    }

    return outputStream;
  }
}

namespace arma {
  bool operator==(
      std::unordered_map<vec, double, mant::Hash, mant::IsEqual> actual,
      const std::unordered_map<vec, double, mant::Hash, mant::IsEqual>& expected) {
    if (actual.size() != expected.size()) {
      return false;
    }

    for (const auto& expectedSample : expected) {
      const auto actualSample = actual.find(expectedSample.first);

      if (actualSample == actual.cend()) {
        return false;
      } else if (std::isnan(expectedSample.second) || std::isnan(actualSample->second)) {
        return false;
      } else if (std::isfinite(expectedSample.second)) {
        if (std::abs(actualSample->second - expectedSample.second) >= ::mant::machinePrecision) {
          return false;
        }
      } else {
        if (!std::isinf(actualSample->second) || std::signbit(actualSample->second) != std::signbit(expectedSample.second)) {
          return false;
        }
      }

      // Makes sure that we wont pick the same sample twice.
      actual.erase(actualSample);
    }

    return true;
  }

  bool operator==(
      const std::vector<vec>& firstVector,
      const std::vector<vec>& secondVector) {
    if (firstVector.size() != secondVector.size()) {
      return false;
    }

    for (uword n = 0; n < firstVector.size(); ++n) {
      if (!approx_equal(firstVector.at(n), secondVector.at(n), "absdiff", ::mant::machinePrecision)) {
        return false;
      }
    }

    return true;
  }

  bool operator==(
      const std::vector<vec::fixed<2>>& firstVector,
      const std::vector<vec::fixed<2>>& secondVector) {
    return (std::vector<vec>(firstVector.cbegin(), firstVector.cend()) == std::vector<vec>(secondVector.cbegin(), secondVector.cend()));
  }

  bool operator==(
      const std::vector<vec::fixed<3>>& firstVector,
      const std::vector<vec::fixed<3>>& secondVector) {
    return (std::vector<vec>(firstVector.cbegin(), firstVector.cend()) == std::vector<vec>(secondVector.cbegin(), secondVector.cend()));
  }

  bool operator==(
      const std::vector<std::pair<vec, double>>& firstVector,
      const std::vector<std::pair<vec, double>>& secondVector) {
    if (firstVector.size() != secondVector.size()) {
      return false;
    }

    for (uword n = 0; n < firstVector.size(); ++n) {
      if (firstVector.at(n).first.has_nan() || secondVector.at(n).first.has_nan()) {
        return false;
      } else if (std::isnan(firstVector.at(n).second) || std::isnan(secondVector.at(n).second)) {
        return false;
      } else if (!approx_equal(firstVector.at(n).first, secondVector.at(n).first, "absdiff", ::mant::machinePrecision)) {
        return false;
      } else if (std::abs(firstVector.at(n).second - secondVector.at(n).second) >= ::mant::machinePrecision) {
        return false;
      }
    }

    return true;
  }

  bool operator!(
      const uvec& vector) {
    return !all(vector);
  }
}

bool hasSameElements(
    std::vector<arma::uvec> actual,
    const std::vector<arma::uvec>& expected) {
  if (actual.size() != expected.size()) {
    return false;
  }

  for (arma::uword n = 0; n < actual.size(); ++n) {
    const auto expectedValues = expected.at(n);
    const auto actualValues = std::find_if(actual.begin(), actual.end(),
        [expectedValues](
                                               const auto& values) {
          if (arma::size(values) != arma::size(expectedValues)) {
            return false;
          }

          return arma::all(values == expectedValues);
        });

    if (actualValues == actual.cend()) {
      return false;
    }

    // Makes sure that we wont pick the same values twice.
    actual.erase(actualValues);
  }

  return true;
}

bool hasSameElements(
    std::vector<std::pair<arma::uvec, arma::uvec>> actual,
    const std::vector<std::pair<arma::uvec, arma::uvec>>& expected) {
  if (actual.size() != expected.size()) {
    return false;
  }

  for (arma::uword n = 0; n < actual.size(); ++n) {
    const auto expectedValues = expected.at(n);
    const auto actualValues = std::find_if(actual.begin(), actual.end(),
        [expectedValues](
                                               const auto& values) {
          if (arma::size(values.first) != arma::size(expectedValues.first) || arma::size(values.second) != arma::size(expectedValues.second)) {
            return false;
          }

          return arma::all(values.first == expectedValues.first) && arma::all(values.second == expectedValues.second);
        });

    if (actualValues == actual.cend()) {
      return false;
    }

    // Makes sure that we wont pick the same values twice.
    actual.erase(actualValues);
  }

  return true;
}

bool isUniformlyDistributed(
    const arma::mat& data,
    const double lowerBound,
    const double upperBound) {
  if (upperBound <= lowerBound) {
    throw std::logic_error("isUniformlyDistributed: The upper bound must be greater than the lower one.");
  } else if (!mant::isRepresentableAsFloatingPoint(data.size())) {
    throw std::overflow_error("isUniformlyDistributed: The data's number of rows must be representable as a floating point.");
  }

  if (data.is_empty() || !std::isfinite(lowerBound) || !std::isfinite(upperBound)) {
    return false;
  }

  // The number of bins is fixed to 10 and 0.001 is used as p-value to calculate the chi-square value for 9 degrees of freedom.
  return arma::all(static_cast<double>(data.n_rows) * arma::rowvec(arma::sum(arma::square(arma::conv_to<arma::mat>::from(arma::umat(arma::histc(data, arma::linspace<arma::vec>(lowerBound, upperBound, 11))).head_rows(10)) / static_cast<double>(data.n_rows) - 0.1) / 0.1)) <= 27.88);
}

bool isUniformlyDistributed(
    const arma::umat& data,
    const arma::uword lowerBound,
    const arma::uword upperBound) {
  if (upperBound <= lowerBound) {
    throw std::logic_error("isUniformlyDistributed: The upper bound must be greater than the lower one.");
  }

  if (data.is_empty()) {
    return false;
  }

  // Uses 0.001 as p-value for the chi-square value.
  const std::array<double, 10> chiSquareValue = {{10.83,
      13.82,
      16.27,
      18.47,
      20.52,
      22.46,
      24.32,
      26.12,
      27.88,
      29.59}};

  return arma::all(static_cast<double>(data.n_rows) * arma::rowvec(arma::sum(arma::square(arma::conv_to<arma::mat>::from(arma::histc(data, arma::regspace<arma::uvec>(lowerBound, upperBound))) / static_cast<double>(data.n_rows) - (1.0 / (upperBound - lowerBound + 1))) / (1.0 / (upperBound - lowerBound + 1)))) <= chiSquareValue.at(upperBound - lowerBound - 1));
}
