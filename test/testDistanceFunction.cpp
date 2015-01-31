// Catch
#include <catch.hpp>
#include <helper.hpp>

// C++ Standard Library
#include <cmath>

// Mantella
#include <mantella>

template <typename ParameterType>
class MockDistanceFunction : public mant::DistanceFunction<ParameterType>{
  public:
    ParameterType getDistanceImplementation(
        const arma::Col<ParameterType>& parameter) const noexcept override;

    arma::Col<ParameterType> getRandomNeighbourImplementation(
        const arma::Col<ParameterType>& parameter,
        const ParameterType& minimalDistance,
        const ParameterType& maximalDistance) const noexcept override;
};

template <>
double MockDistanceFunction<double>::getDistanceImplementation(
    const arma::Col<double>& parameter) const noexcept {
  return arma::norm(parameter, 1);
}

template <>
unsigned int MockDistanceFunction<unsigned int>::getDistanceImplementation(
    const arma::Col<unsigned int>& parameter) const noexcept {
  return arma::accu(parameter);
}

template <>
arma::Col<double> MockDistanceFunction<double>::getRandomNeighbourImplementation(
    const arma::Col<double>& parameter,
    const double& minimalDistance,
    const double& maximalDistance) const noexcept {
  arma::Col<double> result = parameter;
  result.at(0) += (maximalDistance + minimalDistance) / 2.0;
  return result;
}

template <>
arma::Col<unsigned int> MockDistanceFunction<unsigned int>::getRandomNeighbourImplementation(
    const arma::Col<unsigned int>& parameter,
    const unsigned int& minimalDistance,
    const unsigned int& maximalDistance) const noexcept {
  arma::Col<unsigned int> result = parameter;
  result.at(0) += std::floor((maximalDistance + minimalDistance) / 2.0);
  return result;
}

TEST_CASE("DistanceFunction<double>", "") {
  MockDistanceFunction<double> distanceFunction;

  SECTION("Testing getRandomNeighbour(..)") {
    SECTION("Selected random neighbour remains unchanged, considering an unrestricted minimal distance.") {
      compare(distanceFunction.getRandomNeighbour(arma::zeros<arma::Col<double>>(1), 0.0, 2.3), distanceFunction.getRandomNeighbourImplementation(arma::zeros<arma::Col<double>>(1), 0.0, 2.3));
      compare(distanceFunction.getRandomNeighbour(arma::ones<arma::Col<double>>(5), 0.0, 11.32), distanceFunction.getRandomNeighbourImplementation(arma::ones<arma::Col<double>>(5), 0.0, 11.32));
    }

    SECTION("Selected random neighbour remains unchanged, considering an unrestricted minimal distance.") {
      compare(distanceFunction.getRandomNeighbour(arma::zeros<arma::Col<double>>(1), 0.5, 2.3), distanceFunction.getRandomNeighbourImplementation(arma::zeros<arma::Col<double>>(1), 0.5, 2.3));
      compare(distanceFunction.getRandomNeighbour(arma::ones<arma::Col<double>>(5), 5.4, 9.31), distanceFunction.getRandomNeighbourImplementation(arma::ones<arma::Col<double>>(5), 5.4, 9.31));
    }

    SECTION("Selected random neighbour remains unchanged, considering an equal minimal and maximal distance.") {
      compare(distanceFunction.getRandomNeighbour(arma::zeros<arma::Col<double>>(1), 3.0, 3.0), distanceFunction.getRandomNeighbourImplementation(arma::zeros<arma::Col<double>>(1), 3.0, 3.0));
      compare(distanceFunction.getRandomNeighbour(arma::ones<arma::Col<double>>(5), 6.14, 6.14), distanceFunction.getRandomNeighbourImplementation(arma::ones<arma::Col<double>>(5), 6.14, 6.14));
    }

    SECTION("Throws an exception, if minimal distance < 0.") {
      arma::Col<double>::fixed<5> parameter = arma::zeros<arma::Col<double>>(5);
      CHECK_THROWS_AS(distanceFunction.getRandomNeighbour(parameter, -10.3, 4.0), std::logic_error);
    }

    SECTION("Throws an exception, if maximal distance < 0.") {
      arma::Col<double>::fixed<5> parameter = arma::zeros<arma::Col<double>>(5);
      CHECK_THROWS_AS(distanceFunction.getRandomNeighbour(parameter, -8.3, -4.0), std::logic_error);
    }

    SECTION("Throws an exception, if minimal distance > maximal distance.") {
      arma::Col<double>::fixed<5> parameter = arma::zeros<arma::Col<double>>(5);
      CHECK_THROWS_AS(distanceFunction.getRandomNeighbour(parameter, 10.3, 4.0), std::logic_error);
    }
  }

  SECTION("Testing getDistance(...)") {
    SECTION("Distance value remains unchanged.") {
      CHECK(distanceFunction.getDistance(arma::zeros<arma::Col<double>>(1)) == distanceFunction.getDistanceImplementation(arma::zeros<arma::Col<double>>(1)));
      CHECK(distanceFunction.getDistance(arma::Col<double>::fixed<5>({-1.2, 3.0, 1.0, -4.0, 0.0})) == distanceFunction.getDistanceImplementation(arma::Col<double>::fixed<5>({-1.2, 3.0, 1.0, -4.0, 0.0})));
    }
  }
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
      CHECK_THROWS_AS(distanceFunction.getRandomNeighbour(parameter, 10, 4), std::logic_error);
    }
  }

  SECTION("Testing getDistance(...)") {
    SECTION("Distance value remains unchanged.") {
      CHECK(distanceFunction.getDistance(arma::zeros<arma::Col<unsigned int>>(1)) == distanceFunction.getDistanceImplementation(arma::zeros<arma::Col<unsigned int>>(1)));
      CHECK(distanceFunction.getDistance(arma::Col<unsigned int>::fixed<5>({1, 3, 1, 4, 0})) == distanceFunction.getDistanceImplementation(arma::Col<unsigned int>::fixed<5>({1, 3, 1, 4, 0})));
    }
  }
}
