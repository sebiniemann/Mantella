// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("factorial", "[combinatorics][factorial]") {
  GIVEN("A discrete number [n]") {
    WHEN("[n] = 0") {
      THEN("Return 1") {
        CHECK(mant::factorial(0) == 1);
      }
    }

    WHEN("[n] = 1") {
      THEN("Return 1") {
        CHECK(mant::factorial(1) == 1);
      }
    }

    WHEN("[n] = 2") {
      THEN("Return 2") {
        CHECK(mant::factorial(2) == 2);
      }
    }

    WHEN("[n] = 3") {
      THEN("Return 6") {
        CHECK(mant::factorial(3) == 6);
      }
    }

    WHEN("[n] = 4") {
      THEN("Return 24") {
        CHECK(mant::factorial(4) == 24);
      }
    }

    WHEN("[n] = 5") {
      THEN("Return 120") {
        CHECK(mant::factorial(5) == 120);
      }
    }

    WHEN("[n] = 10") {
      THEN("Return 3628800") {
        CHECK(mant::factorial(10) == 3628800);
      }
    }
  }
}

SCENARIO("nchoosek", "[combinatorics][nchoosek]") {
  GIVEN("Two discrete numbers [n] and [k]") {
    WHEN("[n] > [k]") {
      const arma::uword k = discreteRandomNumber();
      CAPTURE(k);

      const arma::uword n = k + discreteRandomNumber();
      CAPTURE(n);

      THEN("Return [n]! / ([n] - [k])! * [k]!") {
        CHECK(mant::nchoosek(n, k) == mant::factorial(n) / (mant::factorial(n - k) * mant::factorial(k)));
      }
    }

    WHEN("[n] = [k]") {
      const arma::uword n = discreteRandomNumber();
      CAPTURE(n);

      const arma::uword k = n;
      CAPTURE(k);

      THEN("Return 1") {
        CHECK(mant::nchoosek(n, k) == 1);
      }
    }

    WHEN("[n] < [k]") {
      const arma::uword n = discreteRandomNumber();
      CAPTURE(n);

      const arma::uword k = n + discreteRandomNumber();
      CAPTURE(k);

      THEN("Return 0") {
        CHECK(mant::nchoosek(n, k) == 0);
      }
    }
  }
}

SCENARIO("combinations", "[combinatorics][combinations]") {
  GIVEN("Two discrete numbers [n] and [k]") {
    WHEN("[n] > [k]") {
      const arma::uword n = 7;
      CAPTURE(n);

      const arma::uword k = 4;
      CAPTURE(k);

      THEN("Return all combinations with size [k], out of [n] elements (in any order).") {
        HAS_SAME_PARAMETERS(mant::combinations(n, k), {{3, 4, 5, 6}, {2, 4, 5, 6}, {2, 3, 5, 6}, {2, 3, 4, 6}, {2, 3, 4, 5}, {1, 4, 5, 6}, {1, 3, 5, 6}, {1, 3, 4, 6}, {1, 3, 4, 5}, {1, 2, 5, 6}, {1, 2, 4, 6}, {1, 2, 4, 5}, {1, 2, 3, 6}, {1, 2, 3, 5}, {1, 2, 3, 4}, {0, 4, 5, 6}, {0, 3, 5, 6}, {0, 3, 4, 6}, {0, 3, 4, 5}, {0, 2, 5, 6}, {0, 2, 4, 6}, {0, 2, 4, 5}, {0, 2, 3, 6}, {0, 2, 3, 5}, {0, 2, 3, 4}, {0, 1, 5, 6}, {0, 1, 4, 6}, {0, 1, 4, 5}, {0, 1, 3, 6}, {0, 1, 3, 5}, {0, 1, 3, 4}, {0, 1, 2, 6}, {0, 1, 2, 5}, {0, 1, 2, 4}, {0, 1, 2, 3}});
      }
    }

    WHEN("[n] = [k]") {
      const arma::uword n = discreteRandomNumber();
      CAPTURE(n);

      const arma::uword k = n;
      CAPTURE(k);

      THEN("Return {0, ..., [n] - 1}.") {
        IS_EQUAL(mant::combinations(n, k), {mant::range(0, n - 1)});
      }
    }

    WHEN("[n] < [k]") {
      const arma::uword n = discreteRandomNumber();
      CAPTURE(n);

      const arma::uword k = n + discreteRandomNumber();
      CAPTURE(k);

      THEN("Return {0, ..., [n] - 1}.") {
        CHECK_THROWS_AS(mant::combinations(n, k), std::logic_error);
      }
    }
  }
}

SCENARIO("multicombinations", "[combinatorics][multicombinations]") {
  GIVEN("Two numbers [n] and [k]") {
    WHEN("[n] > [k]") {
      const arma::uword n = 4;
      CAPTURE(n);

      const arma::uword k = 3;
      CAPTURE(k);

      THEN("Return all multicombinations with size [k], out of [n] elements (in any order).") {
        HAS_SAME_PARAMETERS(mant::multicombinations(n, k), {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}, {1, 1, 0}, {2, 1, 0}, {3, 1, 0}, {2, 2, 0}, {3, 2, 0}, {3, 3, 0}, {1, 1, 1}, {2, 1, 1}, {3, 1, 1}, {2, 2, 1}, {3, 2, 1}, {3, 3, 1}, {2, 2, 2}, {3, 2, 2}, {3, 3, 2}, {3, 3, 3}});
      }
    }

    WHEN("[n] = [k]") {
      const arma::uword n = 3;
      CAPTURE(n);

      const arma::uword k = 3;
      CAPTURE(k);

      THEN("Return all multicombinations with size [k], out of [n] elements (in any order).") {
        HAS_SAME_PARAMETERS(mant::multicombinations(n, k), {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {1, 1, 0}, {2, 1, 0}, {2, 2, 0}, {1, 1, 1}, {2, 1, 1}, {2, 2, 1}, {2, 2, 2}});
      }
    }

    WHEN("[n] < [k]") {
      const arma::uword n = 3;
      CAPTURE(n);

      const arma::uword k = 5;
      CAPTURE(k);

      THEN("Return all multicombinations with size [k], out of [n] elements (in any order).") {
        HAS_SAME_PARAMETERS(mant::multicombinations(3, 5), {{0, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {2, 0, 0, 0, 0}, {1, 1, 0, 0, 0}, {2, 1, 0, 0, 0}, {2, 2, 0, 0, 0}, {1, 1, 1, 0, 0}, {2, 1, 1, 0, 0}, {2, 2, 1, 0, 0}, {2, 2, 2, 0, 0}, {1, 1, 1, 1, 0}, {2, 1, 1, 1, 0}, {2, 2, 1, 1, 0}, {2, 2, 2, 1, 0}, {2, 2, 2, 2, 0}, {1, 1, 1, 1, 1}, {2, 1, 1, 1, 1}, {2, 2, 1, 1, 1}, {2, 2, 2, 1, 1}, {2, 2, 2, 2, 1}, {2, 2, 2, 2, 2}});
      }
    }
  }
}
