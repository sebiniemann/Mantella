#pragma once

// Catch
#include <catch.hpp>

// C++ standard library
#include <string>
#include <unordered_map>
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

/** The root directory of the test data files.
 */
extern std::string rootTestDataDirectory;

/** The MPI node rank (if supported)
 */
extern int nodeRank;

/** The number of MPI nodes (if supported)
 */
extern int numberOfNodes;

/** Randomly generates a (`numberOfRows`, `numberOfColumns`)-matrix of floating point numbers, each within [-100, 100].
 *
 * @return The randomly generated matrix.
 */
arma::Mat<double> getContinuousRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns);

/** Randomly generates a (`numberOfRows`, `numberOfColumns`)-matrix of integers, each within [0, 10].
 *
 * @return The randomly generated matrix.
 */
arma::Mat<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfRows,
    const arma::uword numberOfColumns);

/** Randomly generates a column vector of `numberOfDimensions` floating point numbers, each within [-100, 100].
 *
 * @return The randomly generated column vector.
 */
arma::Col<double> getContinuousRandomNumbers(
    const arma::uword numberOfDimensions);

/** Randomly generates a column vector of `numberOfDimensions` integers, each within [1, 10].
 *
 * @return The randomly generated column vector.
 */
arma::Col<arma::uword> getDiscreteRandomNumbers(
    const arma::uword numberOfDimensions);

/** Randomly generates a floating point number from [-100, 100].
 *
 * @return The randomly generated number.
 */
double getContinuousRandomNumber();

/** Randomly generates an integer from [1, 10].

 *
 * @return The randomly generated number.
 */
arma::uword getDiscreteRandomNumber();

/** Randomly generates an integer from [1, 10], excluding `discreteRandomNumber`.
 *
 * @return The randomly generated number.
 */
arma::uword getDifferentDiscreteRandomNumber(
    const arma::uword discreteRandomNumber);

arma::Mat<double> SYNCHRONISED(
    const arma::Mat<double>& data);

arma::Mat<arma::uword> SYNCHRONISED(
    const arma::Mat<arma::uword>& data);

arma::Col<double> SYNCHRONISED(
    const arma::Col<double>& data);

arma::Col<arma::uword> SYNCHRONISED(
    const arma::Col<arma::uword>& data);

arma::Row<double> SYNCHRONISED(
    const arma::Row<double>& data);

arma::Row<arma::uword> SYNCHRONISED(
    const arma::Row<arma::uword>& data);

double SYNCHRONISED(
    const double data);

arma::uword SYNCHRONISED(
    const arma::uword data);

std::chrono::microseconds SYNCHRONISED(
    const std::chrono::microseconds data);

/** Checks whether `actualSamples` contains the same samples (but maybe in different order) as `expectedSamples` and fails the test if any sample is missing (including duplicates).

 */
void HAS_SAME_SAMPLES(
    const std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual>& actualSamples,
    const std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual>& expectedSamples);

/** Checks whether `actualParameters` contains the same elements (but maybe in different order) as `expectedParameters` and fails the test if any parameter is missing (including duplicates).
 *  This will works also with dimensionally inconsistency between parameters.
 */
void HAS_SAME_PARAMETERS(
    const std::vector<arma::Col<double>>& actualParameters,
    const std::vector<arma::Col<double>>& expectedParameters);

/** Checks whether `actualParameters` contains the same elements (but maybe in different order) as `expectedParameters` and fails the test if any parameter is missing (including duplicates).
 *  This will works also with dimensionally inconsistency between parameters.
 */
void HAS_SAME_PARAMETERS(
    const std::vector<arma::Col<arma::uword>>& actualParameters,
    const std::vector<arma::Col<arma::uword>>& expectedParameters);

/** Checks whether `actualSamples` contains the same parameters (but maybe in different order) as `expectedParameters` and fails the test if any parameter is missing (including duplicates).
 *  The parameters in `actualSamples` are the first elements of each pair.
 *  This will works also with dimensionally inconsistency between parameters.
 */
void HAS_SAME_PARAMETERS(
    const std::vector<std::pair<arma::Col<double>, double>>& actualSamples,
    const std::vector<arma::Col<double>>& expectedParameters);

/** Checks whether `actualElements` contains the same elements (but maybe in different order) as `expectedElements` and fails the test if any element is missing (including duplicates).
 */
void HAS_SAME_ELEMENTS(
    const arma::Col<double>& actualElements,
    const arma::Col<double>& expectedElements);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const std::vector<std::pair<arma::Col<double>, double>>& actual,
    const std::vector<std::pair<arma::Col<double>, double>>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(

    const std::vector<arma::Col<double>>& actual,
    const std::vector<arma::Col<double>>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const std::vector<arma::Col<double>::fixed<3>>& actual,
    const std::vector<arma::Col<double>>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const std::vector<arma::Col<double>::fixed<2>>& actual,
    const std::vector<arma::Col<double>>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const std::vector<arma::Col<arma::uword>>& actual,
    const std::vector<arma::Col<arma::uword>>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(

    const arma::Cube<double>& actual,
    const arma::Cube<double>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Cube<arma::uword>& actual,
    const arma::Cube<arma::uword>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Mat<double>& actual,
    const arma::Mat<double>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Mat<arma::uword>& actual,
    const arma::Mat<arma::uword>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Col<double>& actual,
    const arma::Col<double>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Col<arma::uword>& actual,
    const arma::Col<arma::uword>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Row<double>& actual,
    const arma::Row<double>& expected);

/** Checks whether `actual` contains the same elements, in the same order, as `expected`.
 */
void IS_EQUAL(
    const arma::Row<arma::uword>& actual,
    const arma::Row<arma::uword>& expected);

/** Checks whether all elements in `data` are within [`lowerBound`, `upperBound`] and the distribution of elements in `data` seems to be uniformly.
 */
void IS_UNIFORM(
    const arma::Col<double>& data,
    const double lowerBound,
    const double upperBound);

/** Checks whether all elements in `data` are within [`lowerBound`, `upperBound`] and the distribution of elements in `data` seems to be uniformly.
 */
void IS_UNIFORM(
    const arma::Col<arma::uword>& data,
    const arma::uword lowerBound,
    const arma::uword upperBound);
