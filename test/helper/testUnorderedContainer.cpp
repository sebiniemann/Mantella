// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <unordered_set>

// Mantella
#include <mantella>

TEST_CASE("unorderedContainer: Hash") {
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

TEST_CASE("unorderedContainer: IsKeyEqual") {
  SECTION("Returns true, if both vectors are equal.") {
    const arma::uword numberOfDimensions = getRandomNumberOfValues();
    CAPTURE(numberOfDimensions);
    
    const arma::Col<double>& parameter = getRandomValues(numberOfDimensions, 1);
    CAPTURE(parameter);
  
    mant::IsEqual isEqual;
    CHECK(isEqual(parameter, parameter) == true);
  }

  SECTION("Returns false, if not all elements within the two vectors are equal.") {
    const arma::uword numberOfDimensions = getRandomNumberOfValues();
    CAPTURE(numberOfDimensions);
    
    const arma::Col<double>& firstParameter = getRandomValues(numberOfDimensions, 1);
    CAPTURE(firstParameter);
    const arma::Col<double>& secondParameter = firstParameter - getRandomValues(numberOfDimensions, 1) - 1;
    CAPTURE(secondParameter);
    
    mant::IsEqual isEqual;
    CHECK(isEqual(firstParameter, secondParameter) == false);
  }

  SECTION("Returns false, if the size of the two vectors differ.") {
    const arma::uword firstNumberOfDimensions = getRandomNumberOfValues();
    CAPTURE(firstNumberOfDimensions);
    const arma::Col<double>& firstParameter = getRandomValues(firstNumberOfDimensions, 1);
    CAPTURE(firstParameter);
    
    const arma::uword secondNumberOfDimensions = getDifferentRandomNumberOfValues(firstNumberOfDimensions);
    CAPTURE(secondNumberOfDimensions);
    const arma::Col<double>& secondParameter = getRandomValues(secondNumberOfDimensions, 1);
    CAPTURE(secondParameter);
    
    mant::IsEqual isEqual;
    CHECK(isEqual(firstParameter, secondParameter) == false);
  }
}
