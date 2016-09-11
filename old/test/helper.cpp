#include "testHelper.hpp"

// C++ standard library
#include <array>
#include <cassert>

namespace std {
  ostream& operator<<(
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
    std::vector<arma::uvec> actualElements,
    const std::vector<arma::uvec>& expectedElements) {
  if (actualElements.size() != expectedElements.size()) {
    return false;
  }

  for (arma::uword n = 0; n < actualElements.size(); ++n) {
    const auto expectedValues = expectedElements.at(n);
    const auto actualValues = std::find_if(actualElements.begin(), actualElements.end(), [expectedValues](const auto& values) {
      if (arma::size(values) != arma::size(expectedValues)) {
        return false;
      }

      return arma::all(values == expectedValues);
    });

    if (actualValues == actualElements.cend()) {
      return false;
    }

    // Makes sure that we wont pick the same values twice.
    actualElements.erase(actualValues);
  }

  return true;
}

bool hasSameElements(
    std::vector<std::pair<arma::uvec, arma::uvec>> actualElements,
    const std::vector<std::pair<arma::uvec, arma::uvec>>& expectedElements) {
  if (actualElements.size() != expectedElements.size()) {
    return false;
  }

  for (arma::uword n = 0; n < actualElements.size(); ++n) {
    const auto expectedValues = expectedElements.at(n);
    const auto actualValues = std::find_if(actualElements.begin(), actualElements.end(), [expectedValues](const auto& values) {
      if (arma::size(values.first) != arma::size(expectedValues.first) || arma::size(values.second) != arma::size(expectedValues.second)) {
        return false;
      }

      return arma::all(values.first == expectedValues.first) && arma::all(values.second == expectedValues.second);
    });

    if (actualValues == actualElements.cend()) {
      return false;
    }

    // Makes sure that we wont pick the same values twice.
    actualElements.erase(actualValues);
  }

  return true;
}

bool hasSameDistribution(
    const arma::vec& actualDistribution,
    const arma::vec& expectedDistribution) {
  assert(actualDistribution.is_sorted("ascend"));
  assert(expectedDistribution.is_sorted("ascend"));
  assert(actualDistribution.n_elem == expectedDistribution.n_elem);

  if (actualDistribution.empty() != expectedDistribution.empty()) {
    return false;
  } else if (actualDistribution.empty() && expectedDistribution.empty()) {
    return true;
  }

  if (!mant::isRepresentableAsFloatingPoint(expectedDistribution.n_elem)) {
    throw std::range_error("hasSameDistribution: The number of elements must be representable as a floating point.");
  }

  // Uses a Kolmogorov-Smirnov test with significance level 95%.
  return arma::max(arma::abs(actualDistribution - expectedDistribution)) * std::sqrt(static_cast<double>(expectedDistribution.n_elem) / 2.0) <= std::sqrt(std::log(2.0 / 0.9) / 2.0);
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

  arma::vec uniformDistribution(arma::size(data));
  for (arma::uword n = 0; n < uniformDistribution.n_elem; ++n) {
    uniformDistribution(n) = std::uniform_real_distribution<double>(lowerBound, upperBound)(mant::Rng::getGenerator());
  }

  return hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(data, 10)) / static_cast<double>(data.n_elem)), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(uniformDistribution, 10)) / static_cast<double>(uniformDistribution.n_elem)));
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

  arma::vec normalDistribution(arma::size(data));
  for (arma::uword n = 0; n < normalDistribution.n_elem; ++n) {
    normalDistribution(n) = std::normal_distribution<double>(0.0, standardDeviation)(mant::Rng::getGenerator());
  }

  return hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(data, -100.0, 100.0), 10)) / static_cast<double>(data.n_elem)), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(normalDistribution, -100.0, 100.0), 10)) / static_cast<double>(normalDistribution.n_elem)));
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

  arma::vec cauchyDistribution(arma::size(data));
  for (arma::uword n = 0; n < cauchyDistribution.n_elem; ++n) {
    cauchyDistribution(n) = std::cauchy_distribution<double>(0.0, scale)(mant::Rng::getGenerator());
  }

  return hasSameDistribution(arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(data, -100.0, 100.0), 10)) / static_cast<double>(data.n_elem)), arma::cumsum(arma::conv_to<arma::vec>::from(arma::hist(arma::clamp(cauchyDistribution, -100.0, 100.0), 10)) / static_cast<double>(cauchyDistribution.n_elem)));
}
