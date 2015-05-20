// Catch
#include <catch.hpp>

// C++ Standard Library
#include <unordered_set>

// Mantella
#include <mantella>

TEST_CASE("unorderedContainer: Hash(arma::Col<double>)", "") {
  SECTION("Generates hashes within a low number of collisions.") {
    std::unordered_set<std::size_t> hashes;

    unsigned int numberOfCollisions = 0;
    mant::Hash<double> hash;
    for(unsigned int n = 0; n < 10000; ++n) {
      auto result = hashes.insert(hash(arma::randu<arma::Col<double>>(5)));
      if(!result.second) {
        ++numberOfCollisions;
      }
    }

    CHECK(numberOfCollisions < 10);
  }
}

TEST_CASE("unorderedContainer: Hash(arma::Col<unsigned int>)", "") {
  SECTION("Generates hashes within a low number of collisions.") {
    std::unordered_set<std::size_t> hashes;

    unsigned int numberOfCollisions = 0;
    mant::Hash<unsigned int> hash;
    for(unsigned int n = 0; n < 10000; ++n) {
      auto result = hashes.insert(hash(arma::randi<arma::Col<unsigned int>>(5)));
      if(!result.second) {
        ++numberOfCollisions;
      }
    }

    CHECK(numberOfCollisions < 10);
  }
}

TEST_CASE("unorderedContainer: IsKeyEqual(arma::Col<double>)", "") {
  SECTION("Returns true if both vectors are equal.") {
    mant::IsEqual<double> isEqual;
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4})) == true);
  }

  SECTION("Returns false if not all elements within the two vectors are equal.") {
    mant::IsEqual<double> isEqual;
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 3.9, 5.5, -4.4}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4})) == false);
  }

  SECTION("Returns false if the size of the two vectors differ.") {
    mant::IsEqual<double> isEqual;
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 0.0, 6.5}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4})) == false);
    CHECK(isEqual(arma::Col<double>::fixed<5>({1.0, 0.0, 6.5, -312.4}), arma::Col<double>::fixed<5>({1.0, 0.0, 6.5})) == false);
  }
}

TEST_CASE("unorderedContainer: IsKeyEqual(arma::Col<unsigned int>)", "") {
  SECTION("Returns true if both vectors are equal.") {
    mant::IsEqual<unsigned int> isEqual;
    CHECK(isEqual(arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8, 1203}), arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8, 1203})) == true);
  }

  SECTION("Returns false if not all elements within the two vectors are equal.") {
    mant::IsEqual<unsigned int> isEqual;
    CHECK(isEqual(arma::Col<unsigned int>::fixed<5>({41, 2, 34, 82, 78}), arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8, 1203})) == false);
  }

  SECTION("Returns false if the size of the two vectors differ.") {
    mant::IsEqual<unsigned int> isEqual;
    CHECK(isEqual(arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8}), arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8, 1203})) == false);
    CHECK(isEqual(arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8, 1203}), arma::Col<unsigned int>::fixed<5>({1, 234, 5, 8})) == false);
  }
}
