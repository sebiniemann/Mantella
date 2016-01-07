// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <array>

// Mantella
#include <mantella>

TEST_CASE("hammersleySet") {
  SECTION("Generates a Hammersley sequence without an offset.") {
    arma::Mat<double>::fixed<2, 5> expected = {
        0.0, 0.0, // First point
        1.0 / 2.0,
        1.0 / 3.0, // Second point
        1.0 / 4.0,
        2.0 / 3.0, // Third point
        3.0 / 4.0,
        1.0 / 9.0, // Fourth point
        1.0 / 5.0,
        2.0 / 5.0, // Fifth point
    };
    IS_EQUAL(mant::hammersleySet({2, 3}, {0, 0}, 5), expected);
  }

  SECTION("Generates a Hammersley sequence with an offset.") {
    arma::Mat<double>::fixed<2, 5> expected = {
        3.0 / 4.0, 4.0 / 9.0, // First point
        1.0 / 8.0,
        7.0 / 9.0, // Second point
        5.0 / 8.0,
        2.0 / 9.0, // Third point
        3.0 / 8.0,
        5.0 / 9.0, // Fourth point
        1.0 / 5.0,
        2.0 / 5.0, // Fifth point
    };
    IS_EQUAL(mant::hammersleySet({2, 3}, {3, 4}, 5), expected);
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the size of the base and seed are unequal.") {
      CHECK_THROWS_AS(mant::hammersleySet({1}, {3, 3}, 5), std::logic_error);
      CHECK_THROWS_AS(mant::hammersleySet({4, 5}, {3}, 6), std::logic_error);
    }
  }
}

TEST_CASE("haltonSequence") {
  SECTION("Generates a Halton sequence without an offset.") {
    arma::Mat<double>::fixed<2, 5> expected = {
        0.0, 0.0, // First point
        1.0 / 2.0,
        1.0 / 3.0, // Second point
        1.0 / 4.0,
        2.0 / 3.0, // Third point
        3.0 / 4.0,
        1.0 / 9.0, // Fourth point
        1.0 / 8.0,
        4.0 / 9.0, // Fifth point
    };
    IS_EQUAL(mant::haltonSequence({2, 3}, {0, 0}, 5), expected);
  }

  SECTION("Generates a Halton sequence with an offset.") {
    arma::Mat<double>::fixed<2, 5> expected = {
        3.0 / 4.0, 4.0 / 9.0, // First point
        1.0 / 8.0,
        7.0 / 9.0, // Second point
        5.0 / 8.0,
        2.0 / 9.0, // Third point
        3.0 / 8.0,
        5.0 / 9.0, // Fourth point
        7.0 / 8.0,
        8.0 / 9.0, // Fifth point
    };
    IS_EQUAL(mant::haltonSequence({2, 3}, {3, 4}, 5), expected);
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the size of the base and seed are unequal.") {
      CHECK_THROWS_AS(mant::haltonSequence({1}, {3, 3}, 5), std::logic_error);
      CHECK_THROWS_AS(mant::haltonSequence({4, 5}, {3}, 6), std::logic_error);
    }
  }
}

TEST_CASE("vanDerCorputSequence") {
  SECTION("Generates a Van der Corput sequence without an offset.") {
    IS_EQUAL(mant::vanDerCorputSequence(2, 0, 5), {0.0, 1.0 / 2.0, 1.0 / 4.0, 3.0 / 4.0, 1.0 / 8.0});
  }

  SECTION("Generates a Van der Corput sequence with an offset.") {
    IS_EQUAL(mant::vanDerCorputSequence(3, 3, 5), {1.0 / 9.0, 4.0 / 9.0, 7.0 / 9.0, 2.0 / 9.0, 5.0 / 9.0});
  }
}

TEST_CASE("twoSetsPartitions") {
  SECTION("Generates all unique partitions with exactly two sets.") {
    std::array<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>, 31> expectedTwoSetPartititons = {{{{0}, {1, 2, 3, 4, 5}},
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

    std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> actualTwoSetsPartitions = mant::twoSetsPartitions(6);

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
