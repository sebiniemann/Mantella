#include "catchHelper.hpp"

// C++ standard library
#include <array>
#include <cassert>

namespace std {
  ostream& operator<<(
      ostream& outputStream,
      const unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>& samples) {
    for (const auto& sample : samples) {
      outputStream << "("
                   << accumulate(sample.first.begin() + 1, sample.first.end(), to_string(sample.first(0)), [](const string& concatenatedElements, const double nextElement) {
                        return concatenatedElements + ", " + to_string(nextElement);
                      })
                   << ") => " << sample.second << endl;
    }

    return outputStream;
  }

  ostream&
  operator<<(
      ostream& outputStream,
      const vector<arma::vec>& parameters) {
    for (const auto& parameter : parameters) {
      outputStream << "("
                   << accumulate(parameter.begin() + 1, parameter.end(), to_string(parameter(0)), [](const string& concatenatedElements, const double nextValue) {
                        return concatenatedElements + ", " + to_string(nextValue);
                      })
                   << ")" << endl;
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
      outputStream << "("
                   << accumulate(sample.first.begin() + 1, sample.first.end(), to_string(sample.first(0)), [](const string& concatenatedElements, const double nextElement) {
                        return concatenatedElements + ", " + to_string(nextElement);
                      })
                   << ") => " << sample.second << endl;
    }

    return outputStream;
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::uvec>& elements) {
    for (const auto& element : elements) {
      outputStream << "("
                   << accumulate(element.begin() + 1, element.end(), to_string(element(0)), [](const string& concatenatedElements, const arma::uword nextValue) {
                        return concatenatedElements + ", " + to_string(nextValue);
                      })
                   << ")" << endl;
    }

    return outputStream;
  }

  ostream& operator<<(
      ostream& outputStream,
      const vector<pair<arma::uvec, arma::uvec>>& pairs) {
    const auto elementsConcatenatingFunction = [](
        const string& concatenatedElements,
        const arma::uword nextValue) {
      return concatenatedElements + ", " + to_string(nextValue);
    };

    for (const auto& pair : pairs) {
      outputStream << "("
                   << accumulate(pair.first.begin() + 1, pair.first.end(), to_string(pair.first(0)), elementsConcatenatingFunction) << "), (" << accumulate(pair.second.begin() + 1, pair.second.end(), to_string(pair.second(0)), elementsConcatenatingFunction)
                   << ")" << endl;
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
    return (
        std::vector<vec>(firstVector.cbegin(), firstVector.cend()) == std::vector<vec>(secondVector.cbegin(), secondVector.cend()));
  }

  bool operator==(
      const std::vector<vec::fixed<3>>& firstVector,
      const std::vector<vec::fixed<3>>& secondVector) {
    return (
        std::vector<vec>(firstVector.cbegin(), firstVector.cend()) == std::vector<vec>(secondVector.cbegin(), secondVector.cend()));
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
      } else if (
          std::abs(firstVector.at(n).second - secondVector.at(n).second) >= ::mant::machinePrecision) {
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
    const auto actualValues = std::find_if(actual.begin(), actual.end(), [expectedValues](const auto& values) {
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
    const auto actualValues = std::find_if(actual.begin(), actual.end(), [expectedValues](const auto& values) {
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

bool hasSameDistribution(
    const arma::vec& actualDistribution,
    const arma::vec& expectedDstribution) {
  assert(actualDistribution.is_sorted("ascend"));
  assert(expectedDstribution.is_sorted("ascend"));
  assert(actualDistribution.n_elem == expectedDstribution.n_elem);

  if (actualDistribution.empty() != expectedDstribution.empty()) {
    return false;
  } else if (actualDistribution.empty() && expectedDstribution.empty()) {
    return true;
  }

  if (!mant::isRepresentableAsFloatingPoint(expectedDstribution.n_elem)) {
    throw std::range_error("hasSameDistribution: The number of elements must be representable as a floating point.");
  }

  // Uses a Kolmogorov-Smirnov test with significance level 95%.
  return arma::max(arma::abs(actualDistribution - expectedDstribution)) * std::sqrt(static_cast<double>(expectedDstribution.n_elem) / 2.0) <= std::sqrt(std::log(2.0 / 0.9) / 2.0);
}

bool isUniformDistributed(
    const arma::vec& data,
    const double lowerBound,
    const double upperBound) {
  assert(lowerBound <= upperBound);

  if (data.is_empty()) {
    return false;
  }

  if (!mant::isRepresentableAsFloatingPoint(data.n_elem)) {
    throw std::range_error("isUniformDistributed: The data's number of elements must be representable as a floating point.");
  }

  arma::vec uniformDistribtion(arma::size(data));
  for (arma::uword n = 0; n < uniformDistribtion.n_elem; ++n) {
    uniformDistribtion(n) = std::uniform_real_distribution<double>(lowerBound, upperBound)(mant::Rng::getGenerator());
  }

  return hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(data, 10)) / static_cast<double>(data.n_elem)), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(uniformDistribtion, 10)) / static_cast<double>(uniformDistribtion.n_elem)));
}

bool isNormalDistributed(
    const arma::vec& data,
    const double standardDeviation) {
  assert(standardDeviation >= 0.0);

  if (data.is_empty()) {
    return false;
  }

  if (!mant::isRepresentableAsFloatingPoint(data.n_elem)) {
    throw std::range_error("isNormalDistributed: The data's number of elements must be representable as a floating point.");
  }

  arma::vec normalDistribtion(arma::size(data));
  for (arma::uword n = 0; n < normalDistribtion.n_elem; ++n) {
    normalDistribtion(n) = std::normal_distribution<double>(0.0, standardDeviation)(mant::Rng::getGenerator());
  }

  return hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(data, -100.0, 100.0), 10)) / static_cast<double>(data.n_elem)), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(normalDistribtion, -100.0, 100.0), 10)) / static_cast<double>(normalDistribtion.n_elem)));
}

bool isCauchyDistributed(
    const arma::vec& data,
    const double scale) {
  assert(scale >= 0.0);

  if (data.is_empty()) {
    return false;
  }

  if (!mant::isRepresentableAsFloatingPoint(data.n_elem)) {
    throw std::range_error("isCauchyDistributed: The data's number of elements must be representable as a floating point.");
  }

  arma::vec cauchyDistribtion(arma::size(data));
  for (arma::uword n = 0; n < cauchyDistribtion.n_elem; ++n) {
    cauchyDistribtion(n) = std::cauchy_distribution<double>(0.0, scale)(mant::Rng::getGenerator());
  }

  return hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(data, -100.0, 100.0), 10)) / static_cast<double>(data.n_elem)), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(cauchyDistribtion, -100.0, 100.0), 10)) / static_cast<double>(cauchyDistribtion.n_elem)));
}
