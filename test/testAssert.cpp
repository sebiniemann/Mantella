// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

SCENARIO("verify", "[assert][verify]") {
  GIVEN("An expression and a error message") {
    const std::string& errorMessage = "Some message ...";
    CAPTURE(errorMessage);

    WHEN("The expression is false") {
      const bool expression = false;
      CAPTURE(expression);

      THEN("Throw a std::logic_error") {
        CHECK_THROWS_AS(mant::verify(expression, errorMessage), std::logic_error);
      }
    }

    WHEN("The expression is true") {
      const bool expression = true;
      CAPTURE(expression);

      THEN("Throw no exception") {
        CHECK_NOTHROW(mant::verify(expression, errorMessage));
      }
    }
  }
}

SCENARIO("isRotationMatrix", "[assert][isRotationMatrix]") {
  GIVEN("A matrix") {
    WHEN("The matrix is orthogonal with determinant 1 or -1 (a rotation matrix)") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      arma::Mat<double> matrix = mant::randomRotationMatrix(numberOfDimensions);
      CAPTURE(matrix);

      THEN("Return true") {
        CHECK(mant::isRotationMatrix(matrix) == true);
        // Inverts the determinant's sign
        matrix.swap_cols(0, 1);
        CHECK(mant::isRotationMatrix(matrix) == true);
      }
    }

    WHEN("The matrix is not square") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      const arma::Mat<double>& matrix = arma::randu<arma::Mat<double>>(numberOfDimensions, 1 + numberOfDimensions);
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isRotationMatrix(matrix) == false);
      }
    }

    WHEN("The matrix's determinant is neither 1 or -1") {
      const arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      arma::Mat<double> matrix = mant::randomRotationMatrix(numberOfDimensions);
      // Increasing an element of an orthonormal rotation matrix by 1 should result in a determinant unequal to 1 or -1.
      matrix(0, 0)++;
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isRotationMatrix(matrix) == false);
      }
    }

    WHEN("The matrix's inverse is not equal to its transpose") {
      // We need at least 2 rows and columns for this to work.
      const arma::uword numberOfDimensions = 1 + getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);

      arma::Mat<double> matrix = mant::randomRotationMatrix(numberOfDimensions);
      // Increasing an element of an orthonormal rotation matrix by 1 should result in an inequality transpose and inverse.
      matrix(0, 0)++;
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isRotationMatrix(matrix) == false);
      }
    }
  }
}

SCENARIO("isPermutationVector", "[assert][isPermutationVector]") {
  GIVEN("A vector as well as numbers [cycleSize] and [numberOfElements]") {
    WHEN("[cycleSize] <= [numberOfElements]") {
      const arma::uword cycleSize = getDiscreteRandomNumber();
      CAPTURE(cycleSize);

      const arma::uword numberOfElements = cycleSize + getDiscreteRandomNumber() - 1;
      CAPTURE(numberOfElements);

      AND_WHEN("The vector has [cycleSize] unique elements, all within [0, [numberOfElements] - 1]") {
        const arma::Col<arma::uword>& vector = mant::randomPermutationVector(numberOfElements, cycleSize);
        CAPTURE(vector);

        THEN("Return true") {
          CHECK(mant::isPermutationVector(vector, numberOfElements, cycleSize) == true);
        }
      }

      AND_WHEN("The vector has less then [cycleSize] elements") {
        const arma::Col<arma::uword>& vector = mant::randomPermutationVector(numberOfElements, cycleSize - 1);
        CAPTURE(vector);

        THEN("Return false") {
          CHECK(mant::isPermutationVector(vector, numberOfElements, cycleSize) == false);
        }
      }

      AND_WHEN("The vector has more then [cycleSize] elements") {
        const arma::Col<arma::uword>& vector = arma::randi<arma::Col<arma::uword>>(1 + cycleSize, arma::distr_param(0, static_cast<int>(numberOfElements) - 1));
        CAPTURE(vector);

        THEN("Return false") {
          CHECK(mant::isPermutationVector(vector, numberOfElements, cycleSize) == false);
        }
      }

      AND_WHEN("The vector has at least one element not in [0, [numberOfElements] - 1]") {
        arma::Col<arma::uword> vector = mant::randomPermutationVector(numberOfElements, cycleSize);
        vector(0) = numberOfElements;
        CAPTURE(vector);

        THEN("Return false") {
          CHECK(mant::isPermutationVector(vector, numberOfElements, cycleSize) == false);
        }
      }

      AND_WHEN("The vector has non-unique elements") {
        arma::Col<arma::uword> vector = mant::randomPermutationVector(1 + numberOfElements, 1 + cycleSize);
        vector(0) = vector(1);
        CAPTURE(vector);

        THEN("Return false") {
          CHECK(mant::isPermutationVector(vector, numberOfElements, cycleSize) == false);
        }
      }
    }

    WHEN("[cycleSize] > [numberOfElements]") {
      const arma::uword numberOfElements = getDiscreteRandomNumber();
      CAPTURE(numberOfElements);

      const arma::uword cycleSize = numberOfElements + getDiscreteRandomNumber();
      CAPTURE(cycleSize);

      const arma::Col<arma::uword>& vector = arma::randi<arma::Col<arma::uword>>(cycleSize, arma::distr_param(0, static_cast<int>(numberOfElements) - 1));
      CAPTURE(vector);

      THEN("Return false") {
        CHECK(mant::isPermutationVector(vector, numberOfElements, cycleSize) == false);
      }
    }
  }
}

SCENARIO("isSymmetric", "[assert][isSymmetric]") {
  GIVEN("A matrix") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    WHEN("The matrix is symmetric") {
      const arma::Mat<double>& matrix = arma::symmatu(getContinuousRandomNumbers(numberOfDimensions, numberOfDimensions));
      CAPTURE(matrix);

      THEN("Return true") {
        CHECK(mant::isSymmetric(matrix) == true);
      }
    }

    WHEN("The matrix is not square") {
      const arma::Mat<double>& matrix = arma::randu<arma::Mat<double>>(numberOfDimensions, 1 + numberOfDimensions);
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isSymmetric(matrix) == false);
      }
    }

    WHEN("The matrix has at least one (i,j)-te element that is unequal to its corresponding (j,i)-te element") {
      // We need at least 2 rows and columns for this to occur.
      arma::Mat<double> matrix = arma::symmatu(getContinuousRandomNumbers(1 + numberOfDimensions, 1 + numberOfDimensions));
      matrix(0, 1)++;
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isSymmetric(matrix) == false);
      }
    }
  }
}

SCENARIO("isPositiveSemiDefinite", "[assert][isPositiveSemiDefinite]") {
  GIVEN("A matrix") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);

    WHEN("The matrix is positive semi-definite") {
      arma::Mat<double> matrix = getContinuousRandomNumbers(numberOfDimensions, numberOfDimensions);
      matrix *= matrix.t();
      CAPTURE(matrix);

      THEN("Return true") {
        CHECK(mant::isPositiveSemiDefinite(matrix) == true);
      }
    }

    WHEN("The matrix is not square") {
      const arma::Mat<double>& matrix = arma::randu<arma::Mat<double>>(numberOfDimensions, 1 + numberOfDimensions);
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isSymmetric(matrix) == false);
      }
    }

    WHEN("The matrix has at least one eigenvalue being negative or having an imaginary part") {
      arma::Mat<double> matrix = getContinuousRandomNumbers(numberOfDimensions, numberOfDimensions);
      matrix *= matrix.t();
      CAPTURE(matrix);

      THEN("Return false") {
        CHECK(mant::isPositiveSemiDefinite(-matrix) == false);
      }
    }
  }
}

SCENARIO("isDimensionallyConsistent", "[assert][isDimensionallyConsistent]") {
  GIVEN("A set of samples") {
    const arma::uword numberOfDimensions = SYNCHRONISED(getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);
    mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

    mant::RandomSearch optimisationAlgorithm;
    optimisationAlgorithm.setMaximalNumberOfIterations(100);

    ::mant::isCachingSamples = true;

    optimisationAlgorithm.optimise(optimisationProblem);
    ::mant::isCachingSamples = false;

    WHEN("All samples are dimensionally consistent") {
      const auto& samples = optimisationProblem.getCachedSamples();

      THEN("Return true") {
        CHECK(samples.size() > 0);
        CHECK(mant::isDimensionallyConsistent(samples) == true);
      }
    }

    WHEN("At least to samples have a different number of dimensions between them") {
      const auto& firstSamples = optimisationProblem.getCachedSamples();

      const arma::uword differentNumberOfDimensions = getDifferentDiscreteRandomNumber(numberOfDimensions);
      CAPTURE(differentNumberOfDimensions);
      mant::bbob::SphereFunction secondOptimisationProblem(differentNumberOfDimensions);

      ::mant::isCachingSamples = true;
      optimisationAlgorithm.optimise(secondOptimisationProblem);
      ::mant::isCachingSamples = false;

      const auto& secondSamples = secondOptimisationProblem.getCachedSamples();

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      samples.insert(firstSamples.begin(), firstSamples.end());
      samples.insert(secondSamples.begin(), secondSamples.end());

      THEN("Return false") {
        CHECK(firstSamples.size() > 0);
        CHECK(secondSamples.size() > 0);
        CHECK(samples.size() > 0);
        CHECK(mant::isDimensionallyConsistent(samples) == false);
      }
    }
  }
}
