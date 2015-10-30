// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <array>

// Mantella
#include <mantella>

TEST_CASE("getCombinations") {
  SECTION("Generates all combinations with size k, out of n elements.") {
    std::vector<arma::Col<arma::uword>> expectedCombinations = {
        {3, 4, 5, 6},
        {2, 4, 5, 6},
        {2, 3, 5, 6},
        {2, 3, 4, 6},
        {2, 3, 4, 5},
        {1, 4, 5, 6},
        {1, 3, 5, 6},
        {1, 3, 4, 6},
        {1, 3, 4, 5},
        {1, 2, 5, 6},
        {1, 2, 4, 6},
        {1, 2, 4, 5},
        {1, 2, 3, 6},
        {1, 2, 3, 5},
        {1, 2, 3, 4},
        {0, 4, 5, 6},
        {0, 3, 5, 6},
        {0, 3, 4, 6},
        {0, 3, 4, 5},
        {0, 2, 5, 6},
        {0, 2, 4, 6},
        {0, 2, 4, 5},
        {0, 2, 3, 6},
        {0, 2, 3, 5},
        {0, 2, 3, 4},
        {0, 1, 5, 6},
        {0, 1, 4, 6},
        {0, 1, 4, 5},
        {0, 1, 3, 6},
        {0, 1, 3, 5},
        {0, 1, 3, 4},
        {0, 1, 2, 6},
        {0, 1, 2, 5},
        {0, 1, 2, 4},
        {0, 1, 2, 3}};
    
    HAS_SAME_PARAMETERS(mant::getCombinations(7, 4), expectedCombinations);
  }

  SECTION("Exception tests") {
    SECTION("Throws an exception, if the combination size is larger then the number of elements.") {
      const arma::uword numberOfElements = getRandomNumberOfValues();
      CAPTURE(numberOfElements);

      const arma::uword combinationSize = getRandomNumberOfValues(numberOfElements + 1);
      CAPTURE(combinationSize);
    
      CHECK_THROWS_AS(mant::getCombinations(numberOfElements, combinationSize), std::logic_error);
    }
  }
}

TEST_CASE("getTwoSetsPartitions") {
  SECTION("Generates all unique partitions with exactly two sets.") {
    std::array<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>, 31> expectedTwoSetPartititons = {{
        {{0}, {1, 2, 3, 4, 5}},
        {{1}, {0, 2, 3, 4, 5}},
        {{2}, {0, 1, 3, 4, 5}},
        {{3}, {0, 1, 2, 4, 5}},
        {{4}, {0, 1, 2, 3, 5}},
        {{5}, {0, 1, 2, 3, 4}},
        {{0, 1}, {2, 3, 4, 5}},
        {{0, 2}, {1, 3, 4, 5}},
        {{0, 3}, {1, 2, 4, 5}},
        {{0, 4}, {1, 2, 3, 5}},
        {{0, 5}, {1, 2, 3, 4}},
        {{1, 2}, {0, 3, 4, 5}},
        {{1, 3}, {0, 2, 4, 5}},
        {{1, 4}, {0, 2, 3, 5}},
        {{1, 5}, {0, 2, 3, 4}},
        {{2, 3}, {0, 1, 4, 5}},
        {{2, 4}, {0, 1, 3, 5}},
        {{2, 5}, {0, 1, 3, 4}},
        {{3, 4}, {0, 1, 2, 5}},
        {{3, 5}, {0, 1, 2, 4}},
        {{4, 5}, {0, 1, 2, 3}},
        {{0, 1, 2}, {3, 4, 5}},
        {{0, 1, 3}, {2, 4, 5}},
        {{0, 1, 4}, {2, 3, 5}},
        {{0, 1, 5}, {2, 3, 4}},
        {{0, 2, 3}, {1, 4, 5}},
        {{0, 2, 4}, {1, 3, 5}},
        {{0, 2, 5}, {1, 3, 4}},
        {{0, 3, 4}, {1, 2, 5}},
        {{0, 3, 5}, {1, 2, 4}},
        {{0, 4, 5}, {1, 2, 3}}}};

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> actualTwoSetsPartitions = mant::getTwoSetsPartitions(6);

    REQUIRE(actualTwoSetsPartitions.size() == expectedTwoSetPartititons.size());
    for (arma::uword n = 0; n < expectedTwoSetPartititons.size(); ++n) {
      const std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>& expectedTwoSetsPartition = expectedTwoSetPartititons.at(n);
      bool found = false;
      for (arma::uword k = 0; k < actualTwoSetsPartitions.size(); ++k) {
        const std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>& actualTwoSetsPartition = actualTwoSetsPartitions.at(k);
        if (actualTwoSetsPartition.first.n_elem == expectedTwoSetsPartition.first.n_elem && arma::all(arma::sort(actualTwoSetsPartition.first) == expectedTwoSetsPartition.first) && arma::all(arma::sort(actualTwoSetsPartition.second) == expectedTwoSetsPartition.second)) {
          found = true;
          break;
        }
      }
      CHECK(found == true);
    }
  }
}
