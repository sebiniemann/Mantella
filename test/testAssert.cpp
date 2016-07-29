// Catch
#include <catch.hpp>
#include "catchHelper.hpp"

SCENARIO("isRepresentableAsInteger", "[assert][isRepresentableAsInteger]") {
  GIVEN("A floating point") {
    WHEN("The value is 0") {
      THEN("Return true") {
        CHECK(mant::isRepresentableAsInteger(0.0) == true);
      }
    }

    WHEN("The value is NaN") {
      THEN("Return false") {
        CHECK(mant::isRepresentableAsInteger(std::numeric_limits<double>::quiet_NaN()) == false);
      }
    }

    WHEN("The value is infinity") {
      THEN("Return false") {
        CHECK(mant::isRepresentableAsInteger(std::numeric_limits<double>::infinity()) == false);
      }
    }

    WHEN("The value is not an integer") {
      THEN("Return false") {
        CHECK(mant::isRepresentableAsInteger(2.5) == false);
      }
    }

    WHEN("The value is a negative integer") {
      THEN("Return false") {
        CHECK(mant::isRepresentableAsInteger(-2.0) == false);
      }
    }

    WHEN("The value is an integer and greater than largest supported integer") {
      THEN("Return false") {
        CHECK(mant::isRepresentableAsInteger(static_cast<double>(std::numeric_limits<arma::uword>::max()) * 2.0) == false);
      }
    }

    WHEN("The value is an integer and less than or equal to the largest supported integer") {
      THEN("Return true") {
        CHECK(mant::isRepresentableAsInteger(10.0) == true);
      }
    }
  }
}

SCENARIO("isRepresentableAsFloatingPoint", "[assert][isRepresentableAsFloatingPoint]") {
  GIVEN("An integer") {
    WHEN("The value is 0") {
      THEN("Return true") {
        CHECK(mant::isRepresentableAsFloatingPoint(0) == true);
      }
    }

    WHEN("The value is not representable as a floating point") {
      THEN("Return false") {
        CHECK(mant::isRepresentableAsFloatingPoint(9'007'199'254'740'993) == false);
      }
    }

    WHEN("The value is representable as a floating point") {
      THEN("Return true") {
        CHECK(mant::isRepresentableAsFloatingPoint(10) == true);
      }
    }
  }
}

SCENARIO("isRotationMatrix", "[assert][isRotationMatrix]") {
  GIVEN("A matrix") {
    WHEN("The matrix is empty") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix({}) == false);
      }
    }

    WHEN("The matrix contains NaNs") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 0.0}, {0.0, std::numeric_limits<double>::quiet_NaN()}})) == false);
      }
    }

    WHEN("The matrix contains infinity") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 0.0}, {0.0, std::numeric_limits<double>::infinity()}})) == false);
      }
    }

    WHEN("The matrix has less than 2 dimensions") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix(arma::mat({1.0})) == false);
      }
    }

    WHEN("The matrix is not square") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}})) == false);
      }
    }

    WHEN("The matrix's determinant is neither 1 or -1") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 0.0}, {0.0, -2.0}})) == false);
      }
    }

    WHEN("The matrix's inverse is not equal to its transpose") {
      THEN("Return false") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 0.0}, {0.0, -2.0}})) == false);
      }
    }

    WHEN("The matrix is a rotation matrix with determinant 1") {
      THEN("Return true") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, -1.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, -1.0, 0.0}})) == true);
      }
    }

    WHEN("The matrix is a rotation matrix with determinant -1") {
      THEN("Return true") {
        CHECK(mant::isRotationMatrix(arma::mat({{1.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, -1.0, 0.0}})) == true);
      }
    }
  }
}

SCENARIO("isPermutationVector", "[assert][isPermutationVector]") {
  GIVEN("A vector, a number of elements and a cycle size.") {
    WHEN("The vector is empty") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({}, 10, 1) == false);
      }
    }

    WHEN("The number of elements is 0") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 1}, 0, 2) == false);
      }
    }

    WHEN("The cycle size is 0") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 1}, 10, 0) == false);
      }
    }

    WHEN("The vector's number of elements is less than the cycle size") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 1}, 1, 10) == false);
      }
    }

    WHEN("The vector's elements are not within `{0, ..., numberOfElements - 1}`") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 2}, 2, 2) == false);
      }
    }

    WHEN("The vector's elements are not unique") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 0}, 10, 2) == false);
      }
    }

    WHEN("The vector is a permutation of `cycleSize`-elements from `{0, ..., numberOfElements - 1}`") {
      THEN("Return true") {
        CHECK(mant::isPermutationVector({1, 4, 2, 0}, 10, 4) == true);
      }
    }
  }

  GIVEN("A vector and a number of elements") {
    WHEN("The vector is empty") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({}, 10) == false);
      }
    }

    WHEN("The number of elements is 0") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 1}, 0) == false);
      }
    }

    WHEN("The vector's number of elements is less than the number of elements") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 1}, 1) == false);
      }
    }

    WHEN("The vector's elements are not within `{0, ..., numberOfElements - 1}`") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 2}, 2) == false);
      }
    }

    WHEN("The vector's elements are not unique") {
      THEN("Return false") {
        CHECK(mant::isPermutationVector({0, 0}, 2) == false);
      }
    }

    WHEN("The vector is a permutation of of `(0, ..., numberOfElements - 1)`") {
      THEN("Return true") {
        CHECK(mant::isPermutationVector({1, 4, 2, 0, 3, 5, 9, 7, 8, 6}, 10) == true);
      }
    }
  }
}

SCENARIO("isSymmetric", "[assert][isSymmetric]") {
  GIVEN("A matrix") {
    WHEN("The matrix is empty") {
      THEN("Return false") {
        CHECK(mant::isSymmetric({}) == false);
      }
    }

    WHEN("The matrix contains NaNs") {
      THEN("Return false") {
        CHECK(mant::isSymmetric(arma::mat({{std::numeric_limits<double>::quiet_NaN(), 0.0}, {0.0, 0.0}})) == false);
      }
    }

    WHEN("The matrix has only one element") {
      THEN("Return true") {
        CHECK(mant::isSymmetric(arma::mat({1.0})) == true);
      }
    }

    WHEN("The matrix is not square") {
      THEN("Return false") {
        CHECK(mant::isSymmetric(arma::mat({1.0, 0.0})) == false);
      }
    }

    WHEN("The matrix is not equal to its transpose") {
      THEN("Return false") {
        CHECK(mant::isSymmetric(arma::mat({{0.0, 1.0}, {0.0, 0.0}})) == false);
      }
    }

    WHEN("The matrix's upper and lower triangle differs by less than the defined machine precision") {
      THEN("Return true") {
        CHECK(mant::isSymmetric(arma::mat({{1.0, 0.0}, {std::nexttoward(::mant::machinePrecision, 0.0), 3.0}})) == true);
      }
    }

    WHEN("The matrix is symmetric") {
      THEN("Return true") {
        CHECK(mant::isSymmetric(arma::mat({{1.0, -std::numeric_limits<double>::infinity()}, {-std::numeric_limits<double>::infinity(), 3.0}})) == true);
      }
    }
  }
}

SCENARIO("isPositiveSemiDefinite", "[assert][isPositiveSemiDefinite]") {
  GIVEN("A matrix") {
    WHEN("The matrix is empty") {
      THEN("Return false") {
        CHECK(mant::isPositiveSemiDefinite({}) == false);
      }
    }

    WHEN("The matrix contains NaNs") {
      THEN("Return false") {
        CHECK(mant::isPositiveSemiDefinite(arma::mat({std::numeric_limits<double>::quiet_NaN()})) == false);
      }
    }

    WHEN("The matrix contains infinity") {
      THEN("Return true") {
        CHECK(mant::isPositiveSemiDefinite(arma::mat({std::numeric_limits<double>::infinity()})) == false);
      }
    }

    WHEN("The matrix is not square") {
      THEN("Return false") {
        CHECK(mant::isPositiveSemiDefinite(arma::mat({1.0, 0.0})) == false);
      }
    }

    WHEN("The matrix has not only positive, real eigenvalues") {
      THEN("Return false") {
        CHECK(mant::isPositiveSemiDefinite(arma::mat({{1.0, 0.0}, {0.0, -2.0}})) == false);
      }
    }

    WHEN("The matrix is positive semi-definite") {
      THEN("Return true") {
        CHECK(mant::isPositiveSemiDefinite(arma::mat({{1.0, 0.0}, {0.0, 0.0}})) == true);
      }
    }
  }
}

SCENARIO("isDimensionallyConsistent", "[assert][isDimensionallyConsistent]") {
  GIVEN("A map") {
    WHEN("The map is empty") {
      THEN("Return true") {
        CHECK(mant::isDimensionallyConsistent({}) == true);
      }
    }

    WHEN("The map has two samples with a different number of dimensions") {
      THEN("Return false") {
        CHECK(mant::isDimensionallyConsistent({{{0.0}, -1.0}, {{2.0, -3.0}, 4.0}}) == false);
      }
    }

    WHEN("The map is dimensionally consistent") {
      THEN("Return true") {
        CHECK(mant::isDimensionallyConsistent({{{0.0}, -1.0}, {{-std::numeric_limits<double>::infinity()}, std::numeric_limits<double>::quiet_NaN()}}) == true);
      }
    }
  }
}
