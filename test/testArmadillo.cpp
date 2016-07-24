// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("Hash", "[armadillo][Hash]") {
  GIVEN("A parameter") {
    THEN("Throw no exception and have at most 1 collision between 100.000 randomly independent parameters") {
      mant::Hash hashFunction;

      std::vector<arma::uword> hashes;
      hashes.reserve(100'000);
      bool hasAlreadyCollided = false;
      for (arma::uword n = 0; n < 100'000; ++n) {
        // The actual hash value depends on the used C++ library, wherefore we cannot test is directly.
        const arma::uword hash = hashFunction(mant::uniformRandomNumbers(10));
        if (std::find(hashes.cbegin(), hashes.cend(), hash) != hashes.cend()) {
          CHECK(hasAlreadyCollided == false);
          hasAlreadyCollided = true;
        } else {
          hashes.push_back(hash);
        }
      }
    }
  }
}

SCENARIO("IsEqual", "[armadillo][IsEqual]") {
  GIVEN("Two parameters") {
    WHEN("Both parameters are empty") {
      THEN("Return true") {
        CHECK(mant::IsEqual()({}, {}) == true);
      }
    }

    WHEN("One parameter is a subset of the other one") {
      THEN("Return false") {
        CHECK(mant::IsEqual()(arma::vec({1.0, -2.0}), arma::vec({1.0})) == false);
        CHECK(mant::IsEqual()(arma::vec({1.0}), arma::vec({1.0, -2.0})) == false);
      }
    }

    WHEN("Both parameters contain NaNs") {
      THEN("Return false") {
        CHECK(mant::IsEqual()(arma::vec({std::numeric_limits<double>::quiet_NaN()}), arma::vec({std::numeric_limits<double>::quiet_NaN()})) == false);
      }
    }

    WHEN("Both parameters contain infinity") {
      THEN("Return true") {
        CHECK(mant::IsEqual()(arma::vec({std::numeric_limits<double>::infinity()}), arma::vec({std::numeric_limits<double>::infinity()})) == true);
      }
    }

    WHEN("Both parameters are equal in size but contain different values") {
      THEN("Return false") {
        CHECK(mant::IsEqual()(arma::vec({0.0}), arma::vec({-1.0})) == false);
      }
    }

    WHEN("Both parameters differ by less than the defined machine precision") {
      THEN("Return true") {
        CHECK(mant::IsEqual()(arma::vec({0.0}), arma::vec({std::nexttoward(::mant::machinePrecision, 0.0)})) == true);
      }
    }

    WHEN("Both parameters are identical") {
      THEN("Return true") {
        CHECK(mant::IsEqual()(arma::vec({1.0, -3.0}), arma::vec({1.0, -3.0})) == true);
      }
    }
  }
}
