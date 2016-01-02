// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <unordered_set>

// Mantella
#include <mantella>

TEST_CASE("Hash") {
  SECTION("Generates hashes with a low number of collisions (less than 1 in a million).") {
    std::unordered_set<arma::uword> hashes;

    unsigned int numberOfCollisions = 0;
    mant::Hash hash;
    for (unsigned int n = 0; n < 1000000; ++n) {
      auto result = hashes.insert(hash(arma::randu<arma::Col<double>>(5)));
      if (!result.second) {
        ++numberOfCollisions;
      }
    }

    CHECK(numberOfCollisions < 10);
  }
}

TEST_CASE("IsKeyEqual") {
  SECTION("Returns true, if both vectors are equal.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
    
    const arma::Col<double>& parameter = getContinuousRandomNumbers(numberOfDimensions);
    CAPTURE(parameter);
  
    mant::IsEqual isEqual;
    CHECK(isEqual(parameter, parameter) == true);
  }

  SECTION("Returns false, if not all elements within the two vectors are equal.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
    
    const arma::Col<double>& firstParameter = getContinuousRandomNumbers(numberOfDimensions);
    CAPTURE(firstParameter);
    const arma::Col<double>& secondParameter = firstParameter - getContinuousRandomNumbers(numberOfDimensions) - 1;
    CAPTURE(secondParameter);
    
    mant::IsEqual isEqual;
    CHECK(isEqual(firstParameter, secondParameter) == false);
  }

  SECTION("Returns false, if the size of the two vectors differ.") {
    const arma::uword firstNumberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(firstNumberOfDimensions);
    const arma::Col<double>& firstParameter = getContinuousRandomNumbers(firstNumberOfDimensions);
    CAPTURE(firstParameter);
    
    const arma::uword secondNumberOfDimensions = getDifferentDiscreteRandomNumber(firstNumberOfDimensions);
    CAPTURE(secondNumberOfDimensions);
    const arma::Col<double>& secondParameter = getContinuousRandomNumbers(secondNumberOfDimensions);
    CAPTURE(secondParameter);
    
    mant::IsEqual isEqual;
    CHECK(isEqual(firstParameter, secondParameter) == false);
  }
}

TEST_CASE("range") {
  SECTION("Returns all numbers from *a* to *b*.") {
    SECTION("Works for discrete numbers (*a* < *b*).") {
      const arma::uword start = getDiscreteRandomNumber();
      CAPTURE(start);
      const arma::uword end = start + getDiscreteRandomNumber();
      CAPTURE(end);
    
      IS_EQUAL(mant::range<arma::uword>(start, end), arma::linspace<arma::Col<arma::uword>>(start, end, end - start + 1));
    }
    
    SECTION("Works for continuous numbers (*a* < *b*)") {
      const double start = getContinuousRandomNumber();
      CAPTURE(start);
      const double end = start + std::abs(getContinuousRandomNumber());
      CAPTURE(end);
    
      IS_EQUAL(mant::range<double>(start, end), arma::linspace<arma::Col<double>>(start, end, static_cast<arma::uword>(end - start + 1)));
    }
  }
    
  SECTION("Returns all numbers from *a* to *b* with step size *s*.") {
    SECTION("Works for discrete numbers (*a* > *b*).") {
      const arma::uword end = getDiscreteRandomNumber();
      CAPTURE(end);
      const arma::uword start = end + getDiscreteRandomNumber();
      CAPTURE(start);
      const arma::uword stepSize = getDiscreteRandomNumber();
      CAPTURE(stepSize);
      
      std::vector<arma::uword> expected;
      arma::uword n = 0;
      do {
        expected.push_back(start - n++ * stepSize);
      } while (expected.back() >= end + stepSize);
    
      IS_EQUAL(mant::range<arma::uword>(start, end, stepSize), arma::Col<arma::uword>(expected));
    }
      
    SECTION("Works for continuous numbers (*a* > *b*).") {
      const double end = getContinuousRandomNumber();
      CAPTURE(end);
      const double start = end + std::abs(getContinuousRandomNumber());
      CAPTURE(start);
      // Sets the step size to be at leasts on tenth of the distance, to avoid having to many steps, ultimately running out of memory to store them all.
      const double stepSize = (start - end) / 10.0 + std::abs(getContinuousRandomNumber());
      CAPTURE(stepSize);
      
      std::vector<double> expected;
      arma::uword n = 0;
      do {
        expected.push_back(start - n++ * stepSize);
      } while (expected.back() >= end + stepSize);
    
      IS_EQUAL(mant::range<double>(start, end, stepSize), arma::Col<double>(expected));
    }
  }
}
