// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("hammersleySet", "[numberTheory][hammersleySet]") {
  GIVEN("Some bases and offsets as well as a number of elements") {
    const arma::uword numberOfElements = 5;
    CAPTURE(numberOfElements);

    WHEN("Both the bases and offsets have the same size") {
      const arma::Col<arma::uword>& bases = {2, 3};
      CAPTURE(bases);

      const arma::Col<arma::uword>& offsets = {3, 0};
      CAPTURE(offsets);

      THEN("Return the Hammersley sequence") {
        IS_EQUAL(mant::hammersleySet(bases, offsets, numberOfElements), arma::Mat<double>::fixed<2, 5>({3.0 / 4.0, 0.0 /* First point */, 1.0 / 8.0, 1.0 / 3.0 /* Second point */, 5.0 / 8.0, 2.0 / 3.0 /* Third point */, 3.0 / 8.0, 1.0 / 9.0 /* Fourth point */, 1.0 / 5.0, 2.0 / 5.0 /* Fifth point */}));
      }
    }

    WHEN("The bases and offsets have a different size") {
      const arma::Col<arma::uword>& bases = {1};
      CAPTURE(bases);

      const arma::Col<arma::uword>& offsets = {3, 3};
      CAPTURE(offsets);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::hammersleySet(bases, offsets, numberOfElements), std::logic_error);
      }
    }
  }
}

SCENARIO("haltonSequence", "[numberTheory][haltonSequence]") {
  GIVEN("Some bases and offsets as well as a number of elements") {
    const arma::uword numberOfElements = 5;
    CAPTURE(numberOfElements);

    WHEN("Both the bases and offsets have the same size") {
      const arma::Col<arma::uword>& bases = {2, 3};
      CAPTURE(bases);

      const arma::Col<arma::uword>& offsets = {3, 0};
      CAPTURE(offsets);

      THEN("Return the Halton sequence") {
        IS_EQUAL(mant::haltonSequence(bases, offsets, numberOfElements), arma::Mat<double>::fixed<2, 5>({3.0 / 4.0, 0.0 /* First point */, 1.0 / 8.0, 1.0 / 3.0 /* Second point */, 5.0 / 8.0, 2.0 / 3.0 /* Third point */, 3.0 / 8.0, 1.0 / 9.0 /* Fourth point */, 7.0 / 8.0, 4.0 / 9.0 /* Fifth point */}));
      }
    }

    WHEN("The bases and offsets have a different size") {
      const arma::Col<arma::uword>& bases = {1};
      CAPTURE(bases);

      const arma::Col<arma::uword>& offsets = {3, 3};
      CAPTURE(offsets);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::haltonSequence(bases, offsets, numberOfElements), std::logic_error);
      }
    }
  }
}

SCENARIO("vanDerCorputSequence", "[numberTheory][vanDerCorputSequence]") {
  GIVEN("A base, an offset and a number of elements") {
    const arma::uword base = 2;
    CAPTURE(base);

    const arma::uword offset = 0;
    CAPTURE(offset);

    const arma::uword numberOfElements = 5;
    CAPTURE(numberOfElements);

    THEN("Return the Van der Corput sequence") {
      IS_EQUAL(mant::vanDerCorputSequence(base, offset, numberOfElements), {0.0, 1.0 / 2.0, 1.0 / 4.0, 3.0 / 4.0, 1.0 / 8.0});
    }
  }
}

SCENARIO("twoSetsPartitions", "[numberTheory][twoSetsPartitions]") {
  GIVEN("A number of elements") {
    const arma::uword numberOfElements = 6;
    CAPTURE(numberOfElements);

    THEN("Generates all unique partitions with exactly two sets.") {
      std::vector<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>> actualTwoSetsPartitions = mant::twoSetsPartitions(numberOfElements);
      std::array<std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>, 31> expectedTwoSetPartititons = {{{{0}, {1, 2, 3, 4, 5}}, {{1}, {0, 2, 3, 4, 5}}, {{2}, {0, 1, 3, 4, 5}}, {{3}, {0, 1, 2, 4, 5}}, {{4}, {0, 1, 2, 3, 5}}, {{5}, {0, 1, 2, 3, 4}}, {{0, 1}, {2, 3, 4, 5}}, {{0, 2}, {1, 3, 4, 5}}, {{0, 3}, {1, 2, 4, 5}}, {{0, 4}, {1, 2, 3, 5}}, {{0, 5}, {1, 2, 3, 4}}, {{1, 2}, {0, 3, 4, 5}}, {{1, 3}, {0, 2, 4, 5}}, {{1, 4}, {0, 2, 3, 5}}, {{1, 5}, {0, 2, 3, 4}}, {{2, 3}, {0, 1, 4, 5}}, {{2, 4}, {0, 1, 3, 5}}, {{2, 5}, {0, 1, 3, 4}}, {{3, 4}, {0, 1, 2, 5}}, {{3, 5}, {0, 1, 2, 4}}, {{4, 5}, {0, 1, 2, 3}}, {{0, 1, 2}, {3, 4, 5}}, {{0, 1, 3}, {2, 4, 5}}, {{0, 1, 4}, {2, 3, 5}}, {{0, 1, 5}, {2, 3, 4}}, {{0, 2, 3}, {1, 4, 5}}, {{0, 2, 4}, {1, 3, 5}}, {{0, 2, 5}, {1, 3, 4}}, {{0, 3, 4}, {1, 2, 5}}, {{0, 3, 5}, {1, 2, 4}}, {{0, 4, 5}, {1, 2, 3}}}};

      // Checking whether these partitions are equal or not is very specific and occurs only once, wherefore we decided not to add a helper function for this.
      REQUIRE(actualTwoSetsPartitions.size() == expectedTwoSetPartititons.size());
      for (const auto& expectedTwoSetPartititon : expectedTwoSetPartititons) {
        bool found = false;
        for (arma::uword k = 0; k < actualTwoSetsPartitions.size(); ++k) {
          const std::pair<arma::Col<arma::uword>, arma::Col<arma::uword>>& actualTwoSetsPartition = actualTwoSetsPartitions.at(k);
          if (actualTwoSetsPartition.first.n_elem == expectedTwoSetPartititon.first.n_elem && arma::all(arma::sort(actualTwoSetsPartition.first) == expectedTwoSetPartititon.first) && arma::all(arma::sort(actualTwoSetsPartition.second) == expectedTwoSetPartititon.second)) {
            found = true;
            break;
          }
        }
        CHECK(found == true);
      }
    }
  }
}
