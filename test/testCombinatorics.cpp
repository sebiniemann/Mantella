// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("factorial", "[combinatorics][factorial]") {
  GIVEN("A discrete number") {
    WHEN("The number is 0") {
      THEN("Return 1") {
        CHECK(mant::factorial(0) == 1);
      }
    }

    WHEN("The factorial is not overflowing") {
      THEN("Return the factorial") {
        CHECK(mant::factorial(10) == 3628800);
      }
    }

    WHEN("The factorial is overflowing") {
      THEN("Throw an overflow error") {
        CHECK_THROWS_AS(mant::factorial(100), std::overflow_error);
      }
    }
  }
}

SCENARIO("nchoosek", "[combinatorics][nchoosek]") {
  GIVEN("Two numbers n and k") {
    WHEN("n is 0") {
      THEN("Return 0") {
        CHECK(mant::nchoosek(0, 2) == 0);
      }
    }

    WHEN("k is 0") {
      THEN("Return 1") {
        CHECK(mant::nchoosek(2, 0) == 1);
      }
    }

    WHEN("k is 1") {
      THEN("Return n") {
        CHECK(mant::nchoosek(2, 1) == 2);
      }
    }

    WHEN("n is less than k") {
      THEN("Return 0") {
        CHECK(mant::nchoosek(2, 3) == 0);
      }
    }

    WHEN("n is equal to k") {
      THEN("Return 1") {
        CHECK(mant::nchoosek(3, 3) == 1);
      }
    }

    WHEN("n is greater than k") {
      THEN("Return the binomial coefficient") {
        CHECK(mant::nchoosek(3, 2) == 3);
      }
    }

    WHEN("The binomial coefficient is overflowing") {
      THEN("Throw an overflow error") {
        CHECK_THROWS_AS(mant::nchoosek(100, 50), std::overflow_error);
      }
    }
  }
}

SCENARIO("secondStirlingNumber", "[combinatorics][secondStirlingNumber]") {
  GIVEN("A number of elements and a number of parts") {
    WHEN("The number of elements if 0") {
      THEN("Return 0") {
        CHECK(mant::secondStirlingNumber(0, 1) == 0);
      }
    }

    WHEN("The number of parts is 0") {
      THEN("Return 0") {
        CHECK(mant::secondStirlingNumber(3, 0) == 0);
      }
    }

    WHEN("The number of parts is 1") {
      THEN("Return 1") {
        CHECK(mant::secondStirlingNumber(3, 1) == 1);
      }
    }

    WHEN("The number of elements is less than the number of parts") {
      THEN("Return 0") {
        CHECK(mant::secondStirlingNumber(2, 3) == 0);
      }
    }

    WHEN("The number of elements is equal to the number of parts") {
      THEN("Return 1") {
        CHECK(mant::secondStirlingNumber(3, 3) == 1);
      }
    }

    WHEN("The number of elements is greater than the number of parts") {
      THEN("Return the second Stirling number") {
        CHECK(mant::secondStirlingNumber(3, 2) == 3);
      }
    }

    WHEN("The second Stirling number is overflowing") {
      THEN("Throw an overflow error") {
        CHECK_THROWS_AS(mant::secondStirlingNumber(200, 100), std::overflow_error);
      }
    }
  }
}

SCENARIO("combinations", "[combinatorics][combinations]") {
  GIVEN("Two numbers n and k") {
    WHEN("k is 0") {
      THEN("Return an empty vector") {
        CHECK(mant::combinations(2, 0).empty() == true);
      }
    }

    WHEN("k is 1") {
      THEN("Return the combinations") {
        CHECK(hasSameElements(mant::combinations(2, 1), std::vector<arma::uvec>({{0}, {1}})) == true);
      }
    }

    WHEN("n is equal to k") {
      THEN("Return the combinations") {
        CHECK(hasSameElements(mant::combinations(3, 3), std::vector<arma::uvec>({{0, 1, 2}})) == true);
      }
    }

    WHEN("n is greater than k") {
      THEN("Return the combinations") {
        CHECK(hasSameElements(mant::combinations(3, 2), std::vector<arma::uvec>({{0, 1}, {0, 2}, {1, 2}})) == true);
      }
    }
  }
}

SCENARIO("multicombinations", "[combinatorics][multicombinations]") {
  GIVEN("Two numbers n and k") {
    WHEN("k is 0") {
      THEN("Return an empty vector") {
        CHECK(mant::multicombinations(2, 0).empty() == true);
      }
    }

    WHEN("k is 1") {
      THEN("Return the multi combinations") {
        CHECK(hasSameElements(mant::multicombinations(2, 1), std::vector<arma::uvec>({{0}, {1}})) == true);
      }
    }

    WHEN("n is less than k") {
      THEN("Return the multi combinations") {
        CHECK(hasSameElements(mant::multicombinations(2, 3), std::vector<arma::uvec>({{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {1, 1, 1}})) == true);
      }
    }

    WHEN("n is equal to k") {
      THEN("Return the multi combinations") {
        CHECK(hasSameElements(mant::multicombinations(2, 2), std::vector<arma::uvec>({{0, 0}, {1, 0}, {1, 1}})) == true);
      }
    }

    WHEN("n is greater than k") {
      THEN("Return the multi combinations") {
        CHECK(hasSameElements(mant::multicombinations(3, 2), std::vector<arma::uvec>({{0, 0}, {1, 0}, {2, 0}, {1, 1}, {2, 1}, {2, 2}})) == true);
      }
    }
  }
}

SCENARIO("twoSetsPartitions", "[combinatorics][twoSetsPartitions]") {
  GIVEN("A number of elements") {
    WHEN("The number of elements is greater than 1") {
      THEN("Return all two set partitions") {
        CHECK(hasSameElements(mant::twoSetsPartitions(4), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1, 2, 3}}, {{1}, {0, 2, 3}}, {{2}, {0, 1, 3}}, {{3}, {0, 1, 2}}, {{0, 1}, {2, 3}}, {{0, 2}, {1, 3}}, {{0, 3}, {1, 2}}})) == true);
      }
    }
  }
}
