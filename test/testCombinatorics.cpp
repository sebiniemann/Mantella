// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <array>

// Mantella
#include <mantella>

TEST_CASE("factorial") {
  SECTION("Generates all factorials.") {
    CHECK(mant::factorial(0) == 1);
    CHECK(mant::factorial(1) == 1);
    CHECK(mant::factorial(2) == 2);
    CHECK(mant::factorial(3) == 6);
    CHECK(mant::factorial(4) == 24);
    CHECK(mant::factorial(5) == 120);
    CHECK(mant::factorial(10) == 3628800);
  }
}

TEST_CASE("nchoosek") {
  SECTION("Generates all binomial coefficients.") {
    const arma::uword k = getDiscreteRandomNumber();
    CAPTURE(k);
    const arma::uword n = k + getDiscreteRandomNumber();
    CAPTURE(n);
  
    CHECK(mant::nchoosek(n, k) == mant::factorial(n) / (mant::factorial(n - k) * mant::factorial(k)));
  }
  
  SECTION("Returns 0 if *n* is less than *k*.") {
    const arma::uword n = getDiscreteRandomNumber();
    CAPTURE(n);
    const arma::uword k = n + getDiscreteRandomNumber();
    CAPTURE(k);
  
    CHECK(mant::nchoosek(n, k) == 0);
  }
}

TEST_CASE("combinations") {
  SECTION("Generates all combinations with size *k*, out of *n* elements (*k* != *n*).") {
    std::vector<arma::Col<arma::uword>> expectedCombinations = {
      {3, 4, 5, 6},
      {2, 4, 5, 6},
      {2, 3, 5, 6},
      {2, 3, 4, 6},
      {2, 3, 4, 5},
      {1, 4, 5, 6},
      {1, 3, 5, 6},
      {1, 3, 4, 6},
      {1, 3, 4, 5},
      {1, 2, 5, 6},
      {1, 2, 4, 6},
      {1, 2, 4, 5},
      {1, 2, 3, 6},
      {1, 2, 3, 5},
      {1, 2, 3, 4},
      {0, 4, 5, 6},
      {0, 3, 5, 6},
      {0, 3, 4, 6},
      {0, 3, 4, 5},
      {0, 2, 5, 6},
      {0, 2, 4, 6},
      {0, 2, 4, 5},
      {0, 2, 3, 6},
      {0, 2, 3, 5},
      {0, 2, 3, 4},
      {0, 1, 5, 6},
      {0, 1, 4, 6},
      {0, 1, 4, 5},
      {0, 1, 3, 6},
      {0, 1, 3, 5},
      {0, 1, 3, 4},
      {0, 1, 2, 6},
      {0, 1, 2, 5},
      {0, 1, 2, 4},
      {0, 1, 2, 3}
    };
    
    HAS_SAME_PARAMETERS(mant::combinations(7, 4), expectedCombinations);
  }
  
  SECTION("Generates all combinations with size *k*, out of *n* elements (*k* = *n*).") {
    const arma::uword numberOfElements = getDiscreteRandomNumber();
    CAPTURE(numberOfElements);
  
    HAS_SAME_PARAMETERS(mant::combinations(numberOfElements, numberOfElements), {mant::range<arma::uword>(0, numberOfElements - 1)});
  }

  SECTION("Exception tests:") {
    SECTION("Throws an exception, if the combination size is larger then the number of elements.") {
      const arma::uword numberOfElements = getDiscreteRandomNumber();
      CAPTURE(numberOfElements);
      const arma::uword combinationSize = numberOfElements + getDiscreteRandomNumber();
      CAPTURE(combinationSize);
    
      CHECK_THROWS_AS(mant::combinations(numberOfElements, combinationSize), std::logic_error);
    }
  }
}

TEST_CASE("multicombinations") {
  SECTION("Generates all multicombinations with size *k*, out of *n* elements (*k* < *n*).") {
    std::vector<arma::Col<arma::uword>> expectedMulticombinations = {
      {0, 0, 0},
      {1, 0, 0},
      {2, 0, 0},
      {3, 0, 0},
      {1, 1, 0},
      {2, 1, 0},
      {3, 1, 0},
      {2, 2, 0},
      {3, 2, 0},
      {3, 3, 0},
      {1, 1, 1},
      {2, 1, 1},
      {3, 1, 1},
      {2, 2, 1},
      {3, 2, 1},
      {3, 3, 1},
      {2, 2, 2},
      {3, 2, 2},
      {3, 3, 2},
      {3, 3, 3}
    };
    
    HAS_SAME_PARAMETERS(mant::multicombinations(4, 3), expectedMulticombinations);
  }
  
  SECTION("Generates all multicombinations with size *k*, out of *n* elements (*k* >*n*).") {
    std::vector<arma::Col<arma::uword>> expectedMulticombinations = {
      {0, 0, 0, 0, 0},
      {1, 0, 0, 0, 0},
      {2, 0, 0, 0, 0},
      {1, 1, 0, 0, 0},
      {2, 1, 0, 0, 0},
      {2, 2, 0, 0, 0},
      {1, 1, 1, 0, 0},
      {2, 1, 1, 0, 0},
      {2, 2, 1, 0, 0},
      {2, 2, 2, 0, 0},
      {1, 1, 1, 1, 0},
      {2, 1, 1, 1, 0},
      {2, 2, 1, 1, 0},
      {2, 2, 2, 1, 0},
      {2, 2, 2, 2, 0},
      {1, 1, 1, 1, 1},
      {2, 1, 1, 1, 1},
      {2, 2, 1, 1, 1},
      {2, 2, 2, 1, 1},
      {2, 2, 2, 2, 1},
      {2, 2, 2, 2, 2},
    };
    
    HAS_SAME_PARAMETERS(mant::multicombinations(3, 5), expectedMulticombinations);
  }
}
