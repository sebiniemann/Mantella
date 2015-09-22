// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <unordered_set>

// Mantella
#include <mantella>

TEST_CASE(
    "unorderedContainer: Hash") {
  SECTION(
      "Generates hashes within a low number of collisions.") {
    std::unordered_set<arma::uword> hashes;

    unsigned int numberOfCollisions = 0;
    mant::Hash hash;
    for (unsigned int n = 0; n < 10000; ++n) {
      auto result = hashes.insert(hash(arma::randu<arma::Col<double>>(5)));
      if (!result.second) {
        ++numberOfCollisions;
      }
    }

    CHECK(numberOfCollisions < 10);
  }
}

TEST_CASE(
    "unorderedContainer: IsKeyEqual") {
  SECTION(
      "Returns true if both vectors are equal.") {
    mant::IsEqual isEqual;
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4})) == true);
  }

  SECTION(
      "Returns false if not all elements within the two vectors are equal.") {
    mant::IsEqual isEqual;
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 3.9, 5.5, -4.4}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4})) == false);
  }

  SECTION(
      "Returns false if the size of the two vectors differ.") {
    mant::IsEqual isEqual;
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 0.0, 6.5}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4})) == false);
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5})) == false);
  }
}
