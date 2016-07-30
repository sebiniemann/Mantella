#pragma once

// Mantella
#include <mantella>

namespace std {
  /** Prints a human-friendly representation of the set of samples to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>& samples);

  /** Prints a human-friendly representation of the parameters to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::vec>& parameters);

  /** Prints a human-friendly representation of the parameters to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::vec::fixed<2>>& parameters);

  /** Prints a human-friendly representation of the parameters to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::vec::fixed<3>>& parameters);

  /** Prints a human-friendly representation of the elements to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const vector<pair<arma::vec, double>>& samples);

  /** Prints a human-friendly representation of the elements to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const vector<arma::uvec>& elements);

  /** Prints a human-friendly representation of the elements to the output stream.
   */
  ostream& operator<<(
      ostream& outputStream,
      const vector<pair<arma::uvec, arma::uvec>>& pairs);
}

namespace arma {
  /** Returns true when `actual` contains the same samples (in any order) as `expected` and false otherwise.
   */
  bool operator==(
      std::unordered_map<vec, double, mant::Hash, mant::IsEqual> actual,
      const std::unordered_map<vec, double, mant::Hash, mant::IsEqual>& expected);

  /** Returns true when `actual` is identical to `expected` and false otherwise.
   */
  bool operator==(
      const std::vector<vec>& firstVector,
      const std::vector<vec>& secondVector);

  /** Returns true when `actual` is identical to `expected` and false otherwise.
   */
  bool operator==(
      const std::vector<vec::fixed<2>>& firstVector,
      const std::vector<vec::fixed<2>>& secondVector);

  /** Returns true when `actual` is identical to `expected` and false otherwise.
   */
  bool operator==(
      const std::vector<vec::fixed<3>>& firstVector,
      const std::vector<vec::fixed<3>>& secondVector);

  /** Returns true when `actual` is identical to `expected` and false otherwise.
   */
  bool operator==(
      const std::vector<std::pair<vec, double>>& firstVector,
      const std::vector<std::pair<vec, double>>& secondVector);

  /** Returns true when any value is 0 and false otherwise.
   */
  bool operator!(
      const uvec& vector);
}

/** Returns true when `actual` contains the same values (in any order) as `expected` and false otherwise.
 */
bool hasSameElements(
    std::vector<arma::uvec> actual,
    const std::vector<arma::uvec>& expected);

/** Returns true when `actual` contains the same values (in any order) as `expected` and false otherwise.
 */
bool hasSameElements(
    std::vector<std::pair<arma::uvec, arma::uvec>> actual,
    const std::vector<std::pair<arma::uvec, arma::uvec>>& expected);

/** Returns true when `data` is drawn from a distribution similar to `distribution`.
 */
bool hasSameDistribution(
    const arma::vec& actualDistribution,
    const arma::vec& expectedDistribution);

/** Returns true when `data` is drawn from a uniform distribution and false otherwise.
 */
bool isUniformDistributed(
    const arma::vec& data,
    const double lowerBound,
    const double upperBound);

/** Returns true when `data` is drawn from a normal distribution and false otherwise.
 */
bool isNormalDistributed(
    const arma::vec& data,
    const double standardDeviation);

/** Returns true when `data` is drawn from a Cauchy distribution and false otherwise.
 */
bool isCauchyDistributed(
    const arma::vec& data,
    const double scale);