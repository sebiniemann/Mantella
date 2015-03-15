// Catch
#include <catch.hpp>

// C++ Standard Library
#include <array>

// Mantella
#include <mantella>

TEST_CASE("setTheory: getTwoSetsPartitions(...)", "") {
  SECTION("Generates all unique partitions with exactly two sets.") {
    std::array<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>, 31> expected = {{
      {{0},{1,2,3,4,5}},
      {{1},{0,2,3,4,5}},
      {{2},{0,1,3,4,5}},
      {{3},{0,1,2,4,5}},
      {{4},{0,1,2,3,5}},
      {{5},{0,1,2,3,4}},
      {{0,1},{2,3,4,5}},
      {{0,2},{1,3,4,5}},
      {{0,3},{1,2,4,5}},
      {{0,4},{1,2,3,5}},
      {{0,5},{1,2,3,4}},
      {{1,2},{0,3,4,5}},
      {{1,3},{0,2,4,5}},
      {{1,4},{0,2,3,5}},
      {{1,5},{0,2,3,4}},
      {{2,3},{0,1,4,5}},
      {{2,4},{0,1,3,5}},
      {{2,5},{0,1,3,4}},
      {{3,4},{0,1,2,5}},
      {{3,5},{0,1,2,4}},
      {{4,5},{0,1,2,3}},
      {{0,1,2},{3,4,5}},
      {{0,1,3},{2,4,5}},
      {{0,1,4},{2,3,5}},
      {{0,1,5},{2,3,4}},
      {{0,2,3},{1,4,5}},
      {{0,2,4},{1,3,5}},
      {{0,2,5},{1,3,4}},
      {{0,3,4},{1,2,5}},
      {{0,3,5},{1,2,4}},
      {{0,4,5},{1,2,3}}
    }};

    std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> actual = mant::getTwoSetsPartitions(6);

    REQUIRE(actual.size() == expected.size());
    for(std::size_t n = 0; n < expected.size(); ++n) {
      const std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>& expectedPartition = expected.at(n);
      bool found = false;
      for(std::size_t k = 0; k < actual.size(); ++k) {
        const std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>& actualPartition = actual.at(k);
        if(actualPartition.first.n_elem == expectedPartition.first.n_elem &&
           all(arma::sort(actualPartition.first) == expectedPartition.first) &&
           all(arma::sort(actualPartition.second) == expectedPartition.second)) {
          found = true;
          break;
        }
      }
      CHECK(found == true);
    }
  }
}
