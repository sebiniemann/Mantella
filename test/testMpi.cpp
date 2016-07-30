// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("synchronise", "[mpi][synchronise]") {
  GIVEN("A continuous matrix") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty matrix") {
        arma::mat matrix;

        if (mant::nodeRank() != 0) {
          matrix = {1.0};
        }

        CHECK(mant::synchronise(matrix).is_empty() == true);
      }
    }

    WHEN("The first node's matrix is non-empty") {
      THEN("All nodes return the first node's matrix") {
        arma::mat matrix;

        if (mant::nodeRank() == 0) {
          matrix = arma::mat({{1.0, std::numeric_limits<double>::infinity()}, {-2.0, -std::numeric_limits<double>::infinity()}});
        }

        CHECK(arma::approx_equal(mant::synchronise(matrix), arma::mat({{1.0, std::numeric_limits<double>::infinity()}, {-2.0, -std::numeric_limits<double>::infinity()}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A discrete matrix") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty matrix") {
        arma::umat matrix;

        if (mant::nodeRank() != 0) {
          matrix = {1};
        }

        CHECK(mant::synchronise(matrix).is_empty() == true);
      }
    }

    WHEN("The first node's matrix is non-empty") {
      THEN("All nodes return the first node's matrix") {
        arma::umat matrix;

        if (mant::nodeRank() == 0) {
          matrix = arma::umat({{1, 2}, {3, 4}});
        }

        CHECK(arma::approx_equal(mant::synchronise(matrix), arma::umat({{1, 2}, {3, 4}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A continuous column vector") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty vector") {
        arma::vec vector;

        if (mant::nodeRank() != 0) {
          vector = {1.0};
        }

        CHECK(mant::synchronise(vector).is_empty() == true);
      }
    }

    WHEN("The first node's vector is non-empty") {
      THEN("All nodes return the first node's vector") {
        arma::vec vector;

        if (mant::nodeRank() == 0) {
          vector = arma::vec({1.0, std::numeric_limits<double>::infinity()});
        }
        CHECK(arma::approx_equal(mant::synchronise(vector), arma::vec({1.0, std::numeric_limits<double>::infinity()}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A discrete column vector") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty vector") {
        arma::uvec vector;

        if (mant::nodeRank() != 0) {
          vector = {1};
        }

        CHECK(mant::synchronise(vector).is_empty() == true);
      }
    }

    WHEN("The first node's vector is non-empty") {
      THEN("All nodes return the first node's vector") {
        arma::uvec vector;

        if (mant::nodeRank() == 0) {
          vector = arma::uvec({1, 2});
        }

        CHECK(arma::all(arma::vectorise(mant::synchronise(vector) == arma::uvec({1, 2}))) == true);
      }
    }
  }

  GIVEN("A continuous row vector") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty vector") {
        arma::rowvec vector;

        if (mant::nodeRank() != 0) {
          vector = {1.0};
        }

        CHECK(mant::synchronise(vector).is_empty() == true);
      }
    }

    WHEN("The first node's vector is non-empty") {
      THEN("All nodes return the first node's vector") {
        arma::rowvec vector;

        if (mant::nodeRank() == 0) {
          vector = arma::rowvec({1.0, std::numeric_limits<double>::infinity()});
        }
        CHECK(arma::approx_equal(mant::synchronise(vector), arma::rowvec({1.0, std::numeric_limits<double>::infinity()}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A discrete row vector") {
    WHEN("The first node's matrix is empty") {
      THEN("All nodes return an empty vector") {
        arma::urowvec vector;

        if (mant::nodeRank() != 0) {
          vector = {1};
        }

        CHECK(mant::synchronise(vector).is_empty() == true);
      }
    }

    WHEN("The first node's vector is non-empty") {
      THEN("All nodes return the first node's vector") {
        arma::urowvec vector;

        if (mant::nodeRank() == 0) {
          vector = arma::urowvec({1, 2});
        }

        CHECK(arma::all(arma::vectorise(mant::synchronise(vector) == arma::urowvec({1, 2}))) == true);
      }
    }
  }

  GIVEN("A continuous value") {
    THEN("All nodes return the first node's value") {
      double value = 0.0;

      if (mant::nodeRank() == 0) {
        value = -1.0;
      }

      CHECK(mant::synchronise(value) == Approx(-1.0));
    }
  }

  GIVEN("A discrete value") {
    THEN("All nodes return the first node's value") {
      arma::uword value = 0;

      if (mant::nodeRank() == 0) {
        value = 1;
      }

      CHECK(mant::synchronise(value) == 1);
    }
  }
}
