// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("nodeRank", "[catchHelper][nodeRank]") {
#if defined(SUPPORT_MPI)
  WHEN("MPI is supported") {
    THEN("Contains the node rank") {
      int rank = 0;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      CHECK(::nodeRank == rank);
    }
  }
#else
  WHEN("MPI is not supported") {
    THEN("Contains 0") {
      CHECK(::nodeRank == 0);
    }
  }
#endif
}

SCENARIO("numberOfNodes", "[catchHelper][numberOfNodes]") {
#if defined(SUPPORT_MPI)
  WHEN("MPI is supported") {
    THEN("Contains the number of nodes") {
      int size = 0;
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      CHECK(::numberOfNodes == size);
    }
  }
#else
  WHEN("MPI is not supported") {
    THEN("Contains 1") {
      CHECK(::numberOfNodes == 1);
    }
  }
#endif
}

SCENARIO("operator==", "[catchHelper][operator==]") {
  GIVEN("Two maps") {
    WHEN("One map is empty") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({})) == false);
      }
    }

    WHEN("Both maps are empty") {
      THEN("Return true") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({})) == true);
      }
    }

    WHEN("One map contains NaNs") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::quiet_NaN()}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("Both maps contain NaNs") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::quiet_NaN()}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("One map contains +/-infinity") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{std::numeric_limits<double>::infinity()}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::infinity()}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{std::numeric_limits<double>::infinity()}, 2.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, std::numeric_limits<double>::infinity()}})) == false);
      }
    }

    WHEN("Both maps contain +/-infinity") {
      THEN("Return true") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{std::numeric_limits<double>::infinity()}, 2.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{std::numeric_limits<double>::infinity()}, 2.0}})) == true);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, -std::numeric_limits<double>::infinity()}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{1.0}, -std::numeric_limits<double>::infinity()}})) == true);
      }
    }

    WHEN("One map is a subset of the other one") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0}, -3.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}})) == false);
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0}, -3.0}})) == false);
      }
    }

    WHEN("Both maps contain different samples") {
      THEN("Return false") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{2.0}, -3.0}})) == false);
      }
    }

    WHEN("Both maps are identical (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK((std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0, -3.0}, 4.0}}) == std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual>({{{0.0}, -1.0}, {{2.0, -3.0}, 4.0}})) == true);
      }
    }
  }

  GIVEN("Two vectors of floating points") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({}) == std::vector<arma::vec>({arma::vec({1.0})})) == false);
        CHECK((std::vector<arma::vec>({arma::vec({1.0})}) == std::vector<arma::vec>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({}) == std::vector<arma::vec>({})) == true);
      }
    }

    WHEN("One vector contains NaNs") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::quiet_NaN()})}) == std::vector<arma::vec>({arma::vec({1.0})})) == false);
        CHECK((std::vector<arma::vec>({arma::vec({1.0})}) == std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::quiet_NaN()})})) == false);
      }
    }

    WHEN("Both vectors contain NaNs") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::quiet_NaN()})}) == std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::quiet_NaN()})})) == false);
      }
    }

    WHEN("One vector contains +/-infinity") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::infinity()})}) == std::vector<arma::vec>({arma::vec({1.0})})) == false);
        CHECK((std::vector<arma::vec>({arma::vec({1.0})}) == std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::infinity()})})) == false);
      }
    }

    WHEN("Both vectors contain +/-infinity") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::infinity()})}) == std::vector<arma::vec>({arma::vec({std::numeric_limits<double>::infinity()})})) == true);
      }
    }

    WHEN("One vector is a subset of the other one") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({{0.0}, {2.0}}) == std::vector<arma::vec>({arma::vec({0.0})})) == false);
        CHECK((std::vector<arma::vec>({arma::vec({0.0})}) == std::vector<arma::vec>({{0.0}, {2.0}})) == false);
      }
    }

    WHEN("Both vectors contain different parameters") {
      THEN("Return false") {
        CHECK((std::vector<arma::vec>({arma::vec({0.0})}) == std::vector<arma::vec>({arma::vec({2.0})})) == false);
      }
    }

    WHEN("Both vectors are identical (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({{0.0}, {2.0, -3.0}}) == std::vector<arma::vec>({{0.0}, {2.0, -3.0}})) == true);
      }
    }
  }

  GIVEN("Two vectors of samples") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK((std::vector<arma::vec>({}) == std::vector<arma::vec>({})) == true);
      }
    }

    WHEN("One vector contains NaNs") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::quiet_NaN()}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::quiet_NaN()}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::quiet_NaN()}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("Both vectors contain NaNs") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::quiet_NaN()}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::quiet_NaN()}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::quiet_NaN()}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("One vector contains +/-infinity") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::infinity()}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::infinity()}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::infinity()}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::infinity()}})) == false);
      }
    }

    WHEN("Both vectors contain +/-infinity") {
      THEN("Return true") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::infinity()}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({std::numeric_limits<double>::infinity()}), -2.0}})) == true);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::infinity()}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), std::numeric_limits<double>::infinity()}})) == true);
      }
    }

    WHEN("One vector is a subset of the other one") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0}), -4.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}})) == false);
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0}), -4.0}})) == false);
      }
    }

    WHEN("Both vectors contain different samples") {
      THEN("Return false") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({3.0}), -4.0}})) == false);
      }
    }

    WHEN("Both vectors are identical (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK((std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0, 1.0}), -4.0}}) == std::vector<std::pair<arma::vec, double>>({{arma::vec({1.0}), -2.0}, {arma::vec({3.0, 1.0}), -4.0}})) == true);
      }
    }
  }
}

SCENARIO("operator!", "[catchHelper][operator!]") {
  GIVEN("A discrete vector") {
    WHEN("Any element is 0") {
      THEN("Return true") {
        CHECK(!arma::uvec({0, 1}) == true);
      }
    }

    WHEN("No element is 0") {
      THEN("Return false") {
        CHECK(!arma::uvec({1, 1}) == false);
      }
    }
  }
}

SCENARIO("hasSameElements", "[catchHelper][hasSameElements]") {
  GIVEN("Two vectors of integers") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<arma::uvec>({}), std::vector<arma::uvec>({arma::uvec({0})})) == false);
        CHECK(hasSameElements(std::vector<arma::uvec>({arma::uvec({0})}), std::vector<arma::uvec>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<arma::uvec>({}), std::vector<arma::uvec>({})) == true);
      }
    }

    WHEN("One vector is a proper subset of the other one") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<arma::uvec>({{0}, {1}}), std::vector<arma::uvec>({arma::uvec({0})})) == false);
        CHECK(hasSameElements(std::vector<arma::uvec>({arma::uvec({0})}), std::vector<arma::uvec>({{0}, {1}})) == false);
      }
    }

    WHEN("Both vectors contain different samples") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<arma::uvec>({arma::uvec({0})}), std::vector<arma::uvec>({arma::uvec({1})})) == false);
      }
    }

    WHEN("Both vectors have the same elements (and may be dimensionally inconsistent)") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<arma::uvec>({{0}, {1, 2}}), std::vector<arma::uvec>({{0}, {1, 2}})) == true);
      }
    }
  }

  GIVEN("Two vectors of pairs") {
    WHEN("One vector is empty") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}})) == false);
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({})) == false);
      }
    }

    WHEN("Both vectors are empty") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({}), std::vector<std::pair<arma::uvec, arma::uvec>>({})) == true);
      }
    }

    WHEN("One vector is a proper subset of the other one") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}, {{2}, {3}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}})) == false);
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}, {{2}, {3}}})) == false);
      }
    }

    WHEN("Both vectors contain different samples") {
      THEN("Return false") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{2}, {3}}})) == false);
      }
    }

    WHEN("Both vectors have the same elements (and may be dimensionally inconsistent or in different order)") {
      THEN("Return true") {
        CHECK(hasSameElements(std::vector<std::pair<arma::uvec, arma::uvec>>({{{0}, {1, 2}}, {{3, 4}, {5, 6, 7}}}), std::vector<std::pair<arma::uvec, arma::uvec>>({{{3, 4}, {5, 6, 7}}, {{0}, {1, 2}}})) == true);
      }
    }
  }
}

SCENARIO("isUniformlyDistributed", "[catchHelper][isUniformlyDistributed]") {
  GIVEN("A continuous matrix, a lower and an upper bound") {
    WHEN("The matrix is empty") {
      THEN("Return false") {
        CHECK(isUniformlyDistributed(arma::mat(), 0.0, 1.0) == false);
      }
    }

    WHEN("A bound is infinite") {
      THEN("Return false") {
        CHECK(isUniformlyDistributed(arma::randu<arma::mat>(100000, 10), std::numeric_limits<double>::quiet_NaN(), 1.0) == false);
        CHECK(isUniformlyDistributed(arma::randu<arma::mat>(100000, 10), 0.0, std::numeric_limits<double>::quiet_NaN()) == false);
        CHECK(isUniformlyDistributed(arma::randu<arma::mat>(100000, 10), -std::numeric_limits<double>::infinity(), 1.0) == false);
        CHECK(isUniformlyDistributed(arma::randu<arma::mat>(100000, 10), 0.0, std::numeric_limits<double>::infinity()) == false);
      }
    }

    WHEN("The lower bound is greater than the upper one") {
      THEN("Throw a logic error") {
        CHECK_THROWS_AS(isUniformlyDistributed(arma::randu<arma::mat>(100000, 10), 1.0, 0.0), std::logic_error);
      }
    }

    WHEN("The matrix is not uniformly drawn from `[lowerBound, upperBound]`") {
      THEN("Return false") {
        CHECK(isUniformlyDistributed(arma::randn<arma::mat>(100000, 10), 0.0, 1.0) == false);
      }
    }

    WHEN("The matrix is uniformly drawn from `[lowerBound, upperBound]`") {
      THEN("Return true") {
        CHECK(isUniformlyDistributed(arma::randu<arma::mat>(100000, 10), 0.0, 1.0) == true);
      }
    }
  }

  GIVEN("A discrete matrix, a lower and an upper bound") {
    WHEN("The matrix is empty") {
      THEN("Return false") {
        CHECK(isUniformlyDistributed(arma::umat(), 0, 1) == false);
      }
    }

    WHEN("The lower bound is greater than the upper one") {
      THEN("Throw a logic error") {
        CHECK_THROWS_AS(isUniformlyDistributed(arma::randi<arma::umat>(100000, 10, arma::distr_param(0, 1)), 1, 0), std::logic_error);
      }
    }

    WHEN("The matrix is not uniformly drawn from `[lowerBound, upperBound]`") {
      THEN("Return false") {
        CHECK(isUniformlyDistributed(arma::randi<arma::umat>(100000, 10, arma::distr_param(0, 1)), 0, 2) == false);
      }
    }

    WHEN("The matrix is uniformly drawn from `[lowerBound, upperBound]`") {
      THEN("Return true") {
        CHECK(isUniformlyDistributed(arma::randi<arma::umat>(100000, 10, arma::distr_param(0, 2)), 0, 2) == true);
      }
    }
  }
}
