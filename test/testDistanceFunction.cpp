// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cmath>

// Mantella
#include <mantella>

#include "./helper.hpp"

template <typename T>
class MockDistanceFunction : public mant::DistanceFunction<T>{
  public:
    T getLengthImplementation(
        const arma::Col<T>& parameter) const noexcept override;

    arma::Col<T> getRandomNeighbourImplementation(
        const arma::Col<T>& parameter,
        const T minimalDistance,
        const T maximalDistance) const noexcept override;
};

template <typename T>
T MockDistanceFunction<T>::getLengthImplementation(
    const arma::Col<T>& parameter) const noexcept {
  return arma::accu(parameter);
}

template <typename T>
arma::Col<T> MockDistanceFunction<T>::getRandomNeighbourImplementation(
    const arma::Col<T>& parameter,
    const T minimalDistance,
    const T maximalDistance) const noexcept {
  arma::Col<T> result = parameter;
  result.at(0) += std::floor((maximalDistance + minimalDistance) / 2.0);
  return result;
}

TEST_CASE("DistanceFunction<unsigned int>", "") {
  MockDistanceFunction<unsigned int> distanceFunction;

  SECTION("Testing getRandomNeighbour(..)") {
    SECTION("Selected random neighbour remains unchanged, considering an unrestricted minimal distance.") {
      compare(distanceFunction.getRandomNeighbour(arma::zeros<arma::Col<unsigned int>>(1), 0, 2), distanceFunction.getRandomNeighbourImplementation(arma::zeros<arma::Col<unsigned int>>(1), 0, 2));
      compare(distanceFunction.getRandomNeighbour(arma::ones<arma::Col<unsigned int>>(5), 0, 12), distanceFunction.getRandomNeighbourImplementation(arma::ones<arma::Col<unsigned int>>(5), 0, 12));
    }

    SECTION("Selected random neighbour remains unchanged, considering an unrestricted minimal distance.") {
      compare(distanceFunction.getRandomNeighbour(arma::zeros<arma::Col<unsigned int>>(1), 1, 2), distanceFunction.getRandomNeighbourImplementation(arma::zeros<arma::Col<unsigned int>>(1), 1, 2));
      compare(distanceFunction.getRandomNeighbour(arma::ones<arma::Col<unsigned int>>(5), 11, 22), distanceFunction.getRandomNeighbourImplementation(arma::ones<arma::Col<unsigned int>>(5), 11, 22));
    }

    SECTION("Selected random neighbour remains unchanged, considering an equal minimal and maximal distance.") {
      compare(distanceFunction.getRandomNeighbour(arma::zeros<arma::Col<unsigned int>>(1), 3, 3), distanceFunction.getRandomNeighbourImplementation(arma::zeros<arma::Col<unsigned int>>(1), 3, 3));
      compare(distanceFunction.getRandomNeighbour(arma::ones<arma::Col<unsigned int>>(5), 71, 71), distanceFunction.getRandomNeighbourImplementation(arma::ones<arma::Col<unsigned int>>(5), 71, 71));
    }

    SECTION("Throws an exception, if minimal distance > maximal distance.") {
      arma::Col<unsigned int>::fixed<5> parameter = arma::zeros<arma::Col<unsigned int>>(5);
      CHECK_THROWS_AS(distanceFunction.getRandomNeighbour(parameter, 10, 4), std::runtime_error);
    }
  }

  SECTION("Testing getDistance(...)") {
    SECTION("Distance value remains unchanged.") {
      CHECK(distanceFunction.getLength(arma::zeros<arma::Col<unsigned int>>(1)) == 0);
      CHECK(distanceFunction.getLength(arma::Col<unsigned int>::fixed<5>({1, 3, 1, 4, 0})) == 9);
    }
  }
}
