// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("hammersleySet", "[numberTheory][hammersleySet]") {
  GIVEN("Bases, seeds and a number of elements") {
    WHEN("The number of elements is 0") {
      THEN("Return an empty sequence") {
        CHECK(mant::hammersleySet({2}, {1}, 0).is_empty() == true);
      }
    }

    WHEN("Both the bases and seeds have the same size") {
      THEN("Return the Hammersley sequence") {
        CHECK(arma::approx_equal(mant::hammersleySet({2, 3}, {3, 0}, 5), arma::mat({{0.75, 0.125, 0.625, 0.375, 0.2}, {0.0, 1.0 / 3.0, 2.0 / 3.0, 1.0 / 9.0, 0.4}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("Bases and a number of elements") {
    WHEN("The number of elements is 0") {
      THEN("Return an empty sequence") {
        CHECK(mant::hammersleySet({2}, 0).is_empty() == true);
      }
    }

    WHEN("Both the bases and seeds have the same size") {
      THEN("Return the Hammersley sequence") {
        CHECK(arma::approx_equal(mant::hammersleySet({2, 3}, 5), arma::mat({{0.0, 0.5, 0.25, 0.75, 0.2}, {0.0, 1.0 / 3.0, 2.0 / 3.0, 1.0 / 9.0, 0.4}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}

SCENARIO("haltonSequence", "[numberTheory][haltonSequence]") {
  GIVEN("Bases, seeds and a number of elements") {
    WHEN("The number of elements is 0") {
      THEN("Return an empty sequence") {
        CHECK(mant::haltonSequence({2}, {1}, 0).is_empty() == true);
      }
    }

    WHEN("Both the bases and seeds have the same size") {
      THEN("Return the Halton sequence") {
        CHECK(arma::approx_equal(mant::haltonSequence({2, 3}, {3, 0}, 5), arma::mat({{0.75, 0.125, 0.625, 0.375, 0.875}, {0.0, 1.0 / 3.0, 2.0 / 3.0, 1.0 / 9.0, 4.0 / 9.0}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("Bases and a number of elements") {
    WHEN("The number of elements is 0") {
      THEN("Return an empty sequence") {
        CHECK(mant::haltonSequence({2}, 0).is_empty() == true);
      }
    }

    WHEN("Both the bases and seeds have the same size") {
      THEN("Return the Halton sequence") {
        CHECK(arma::approx_equal(mant::haltonSequence({2, 3}, 5), arma::mat({{0.0, 0.5, 0.25, 0.75, 0.125}, {0.0, 1.0 / 3.0, 2.0 / 3.0, 1.0 / 9.0, 4.0 / 9.0}}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}

SCENARIO("vanDerCorputSequence", "[numberTheory][vanDerCorputSequence]") {
  GIVEN("A base, a seed and a number of elements") {
    WHEN("The seed is 0") {
      THEN("Return the Van der Corput sequence") {
        CHECK(arma::approx_equal(mant::vanDerCorputSequence(2, 0, 2), arma::vec({0.0, 0.5}), "absdiff", ::mant::machinePrecision) == true);
      }
    }

    WHEN("The number of elements is 0") {
      THEN("Return an empty sequence") {
        CHECK(mant::vanDerCorputSequence(2, 1, 0).is_empty() == true);
      }
    }

    WHEN("The base is greater than or equal to 2") {
      THEN("Return the Van der Corput sequence") {
        CHECK(arma::approx_equal(mant::vanDerCorputSequence(2, 1, 4), arma::vec({0.5, 0.25, 0.75, 0.125}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }

  GIVEN("A base and a number of elements") {
    WHEN("The number of elements is 0") {
      THEN("Return an empty sequence") {
        CHECK(mant::vanDerCorputSequence(2, 0).is_empty() == true);
      }
    }

    WHEN("The base is greater than or equal to 2") {
      THEN("Return the Van der Corput sequence") {
        CHECK(arma::approx_equal(mant::vanDerCorputSequence(2, 4), arma::vec({0.0, 0.5, 0.25, 0.75}), "absdiff", ::mant::machinePrecision) == true);
      }
    }
  }
}
