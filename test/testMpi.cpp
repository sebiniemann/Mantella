// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("synchronise", "[mpi][synchronise]") {
  GIVEN("A continuous matrix") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty matrix") {
        arma::mat matrix;

        if (::nodeRank != 0) {
          matrix = {1.0};
        }

        CHECK(mant::synchronise(matrix).is_empty() == true);
      }
    }

    WHEN("The first node's matrix is non-empty") {
      THEN("All nodes return the first node's matrix") {
        arma::mat matrix;

        if (::nodeRank == 0) {
          matrix = arma::mat({{1.0, std::numeric_limits<double>::infinity()}, {-2.0, -std::numeric_limits<double>::infinity()}});
        }

        CHECK(arma::approx_equal(mant::synchronise(matrix), arma::mat({{1.0, std::numeric_limits<double>::infinity()}, {-2.0, -std::numeric_limits<double>::infinity()}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A continuous vector") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty vector") {
        arma::vec vector;

        if (::nodeRank != 0) {
          vector = {1.0};
        }

        CHECK(mant::synchronise(vector).is_empty() == true);
      }
    }

    WHEN("The first node's vector is non-empty") {
      THEN("All nodes return the first node's vector") {
        arma::vec vector;

        if (::nodeRank == 0) {
          vector = arma::vec({1.0, std::numeric_limits<double>::infinity()});
        }
        CHECK(arma::approx_equal(mant::synchronise(vector), arma::vec({1.0, std::numeric_limits<double>::infinity()}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A discrete vector") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty vector") {
        arma::uvec vector;

        if (::nodeRank != 0) {
          vector = {1};
        }

        CHECK(mant::synchronise(vector).is_empty() == true);
      }
    }

    WHEN("The first node's vector is non-empty") {
      THEN("All nodes return the first node's vector") {
        arma::uvec vector;

        if (::nodeRank == 0) {
          vector = arma::uvec({1});
        }
        CHECK(arma::all(arma::vectorise(mant::synchronise(vector) == arma::uvec({1}))) == true);
      }
    }
  }

  GIVEN("A continuous value") {
    THEN("All nodes return the first node's value") {
      double value = 0.0;

      if (::nodeRank == 0) {
        value = -1.0;
      }

      CHECK(mant::synchronise(value) == Approx(-1.0));
    }
  }

  GIVEN("A discrete value") {
    THEN("All nodes return the first node's value") {
      arma::uword value = 0;

      if (::nodeRank == 0) {
        value = 1;
      }

      CHECK(mant::synchronise(value) == 1);
    }
  }
}
