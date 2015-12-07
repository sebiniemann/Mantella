// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

// Armadillo
#include <armadillo>

TEST_CASE("getDiscreteRandomNumber") {
  SECTION("Generates discrete random numbers within [1, 10].") {
    arma::Col<arma::uword>::fixed<10000> randomNumbers;
    for (arma::uword n = 0; n < randomNumbers.n_elem; ++n) {
      randomNumbers(n) = getDiscreteRandomNumber();
    }
    
    IS_UNIFORM(randomNumbers, 1, 10);
  }
}

TEST_CASE("getDifferentDiscreteRandomNumber") {
  SECTION("Generates discrete random numbers within [1, 10], excluding the provided number.") {
    arma::Col<arma::uword>::fixed<10000> randomNumbers;
    for (arma::uword n = 0; n < randomNumbers.n_elem; ++n) {
      arma::uword randomNumberToExclude = getDiscreteRandomNumber();
      arma::uword differentRandomNumber = getDifferentDiscreteRandomNumber(randomNumberToExclude);
      
      CAPTURE(randomNumberToExclude);
      CAPTURE(differentRandomNumber);
      
      CHECK(differentRandomNumber != randomNumberToExclude);
      randomNumbers(n) = differentRandomNumber;
    }
    
    // Since we uniformly exclude a number from [1, 10] for each draw, the whole set of numbers should again be uniform.
    IS_UNIFORM(randomNumbers, 1, 10);
  }
}

TEST_CASE("getContinuousRandomNumber") {
  SECTION("Generates discrete random numbers within [-100, 100].") {
    arma::Col<double>::fixed<10000> randomNumbers;
    for (arma::uword n = 0; n < randomNumbers.n_elem; ++n) {
      randomNumbers(n) = getContinuousRandomNumber();
    }
    
    IS_UNIFORM(randomNumbers, -100.0, 100.0);
  }
}

TEST_CASE("getDiscreteRandomNumbers") {
  SECTION("Generates discrete random vectors within [1, 10]^n.") {
    IS_UNIFORM(getDiscreteRandomNumbers(10000), 1, 10);
  }
  
  SECTION("Generates discrete random matrices within [1, 10]^n.") {
    IS_UNIFORM(arma::vectorise(getDiscreteRandomNumbers(1000, 10)), 1, 10);
  }
}

TEST_CASE("getContinuousRandomNumbers") {
  SECTION("Generates continuous random vectors within [-100, 100]^n.") {
    IS_UNIFORM(getContinuousRandomNumbers(10000), -100, 100);
  }
  
  SECTION("Generates continuous random matrices within [-100, 100]^n.") {
    IS_UNIFORM(arma::vectorise(getContinuousRandomNumbers(1000, 10)), -100, 100);
  }
}

// As we use Catch's CHECK procedure within all of the following functions, checking whether an invalid result would be detected will always fail the whole test (even if we wanted this particular instruction to fail).
TEST_CASE("HAS_SAME_SAMPLES") {
  SECTION("Works between two unordered maps, even if the parameters are not dimensionally consistent.") {
    const arma::uword numberOfParameters = getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> unorderedMap;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      unorderedMap.insert({getContinuousRandomNumbers(getDiscreteRandomNumber()), getContinuousRandomNumber()});
    }
    
    HAS_SAME_SAMPLES(unorderedMap, unorderedMap);
  }
}

TEST_CASE("HAS_SAME_PARAMETERS") {
  SECTION("Works between two vectors, even if the parameters are not dimensionally consistent.") {
    const arma::uword numberOfParameters = getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    std::vector<arma::Col<arma::uword>> parameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      parameters.push_back(getDiscreteRandomNumbers(getDiscreteRandomNumber()));
    }
    
    std::vector<arma::Col<arma::uword>> shuffeldParameters = parameters;
    std::random_shuffle(shuffeldParameters.begin(), shuffeldParameters.end());
    
    HAS_SAME_PARAMETERS(shuffeldParameters, parameters);
  }
  
  SECTION("Works between a vector of parameters and a vector of samples, even if the parameters are not dimensionally consistent.") {
    const arma::uword numberOfParameters = getDiscreteRandomNumber();
    CAPTURE(numberOfParameters);
    
    std::vector<arma::Col<double>> parameters;
    for (arma::uword n = 0; n < numberOfParameters; ++n) {
      parameters.push_back(getContinuousRandomNumbers(getDiscreteRandomNumber()));
    }
    
    std::vector<std::pair<arma::Col<double>, double>> samples;
    for (const auto& parameter : parameters) {
      samples.push_back({parameter, getContinuousRandomNumber()});
    }
    std::random_shuffle(samples.begin(), samples.end());
    
    HAS_SAME_PARAMETERS(samples, parameters);
  }
}

TEST_CASE("HAS_SAME_ELEMENTS") {
  SECTION("Works for continuous vectors.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
  
    const arma::Col<double>& parameter = getContinuousRandomNumbers(numberOfDimensions);
    HAS_SAME_ELEMENTS(parameter, arma::shuffle(parameter));
  }
}

TEST_CASE("IS_EQUAL") {
  SECTION("Works for continuous cubes.") {
    const arma::uword numberOfRows = getDiscreteRandomNumber();
    CAPTURE(numberOfRows);
    const arma::uword numberOfColumns = getDiscreteRandomNumber();
    CAPTURE(numberOfColumns);
    const arma::uword numberOfSlices = getDiscreteRandomNumber();
    CAPTURE(numberOfSlices);
  
    // Currently, we don't provide a function to deliver random numbers for cubes.
    const arma::Cube<double>& parameters = arma::randu<arma::Cube<double>>(numberOfRows, numberOfColumns, numberOfSlices);
    IS_EQUAL(parameters, parameters);
  }
  
  SECTION("Works for continuous matrices.") {
    const arma::uword numberOfRows = getDiscreteRandomNumber();
    CAPTURE(numberOfRows);
    const arma::uword numberOfColumns = getDiscreteRandomNumber();
    CAPTURE(numberOfColumns);
  
    const arma::Mat<double>& parameters = getContinuousRandomNumbers(numberOfRows, numberOfColumns);
    IS_EQUAL(parameters, parameters);
  }
  
  SECTION("Works for discrete vectors.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
  
    const arma::Col<arma::uword>& parameter = getDiscreteRandomNumbers(numberOfDimensions);
    IS_EQUAL(parameter, parameter);
  }
  
  SECTION("Works for continuous vectors.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
  
    const arma::Col<double>& parameter = getContinuousRandomNumbers(numberOfDimensions);
    IS_EQUAL(parameter, parameter);
  }
}

// We can't use any of the above generators for this, since we use *IS_UNIFORM* to test these generators and would run into circular reasoning otherwise.
TEST_CASE("IS_UNIFORM") {
  SECTION("Works for discrete vectors.") {
    IS_UNIFORM(arma::randi<arma::Col<arma::uword>>(10000, arma::distr_param(0, 100)), 0, 100);
  }
  
  SECTION("Works for continuous vectors.") {
    IS_UNIFORM(arma::randu<arma::Col<double>>(10000) * 200.0 - 100.0, -100.0, 100.0);
  }
}
