// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <memory>
#include <random>
#include <stdexcept>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("verify") {
  SECTION("Throws std::logic_error, if the expression is false.") {
    CHECK_THROWS_AS(mant::verify(false, "Some message ..."), std::logic_error);
  }

  SECTION("Does nothing, if the expression is true.") {
    CHECK_NOTHROW(mant::verify(true, "Some message ..."));
  }
}

TEST_CASE("isRotationMatrix") {
  // Some tests expect that the matrix has at least 2 columns/rows.
  const arma::uword numberOfDimensions = 1 + getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  arma::Mat<double> rotationMatrix = mant::randomRotationMatrix(numberOfDimensions);
  CAPTURE(rotationMatrix);
    
  SECTION("Returns true, if the matrix is a rotation matrix.") {
    CHECK(mant::isRotationMatrix(rotationMatrix) == true);
  }

  SECTION("Returns false, if the matrix is not a rotation matrix.") {
    SECTION("Returns false, if the matrix is not square.") {
      rotationMatrix.shed_col(0);
      CAPTURE(rotationMatrix);

      CHECK(mant::isRotationMatrix(rotationMatrix) == false);
    }

    SECTION("Returns false, if the matrix has not an determinant of (nearly) -1 or 1.") {
      // Increasing any element of a orthonormal rotation matrix by 1 should result in a determinant unequal to 1 or -1.
      rotationMatrix(0, 0)++;
      CAPTURE(rotationMatrix);

      CHECK(mant::isRotationMatrix(rotationMatrix) == false);
    }

    SECTION("Returns false, if its transpose is not (nearly) equal to its inverse.") {
      // Increasing any element of a orthonormal rotation matrix by 1 should result in an inequality transpose and inverse.
      rotationMatrix(0, rotationMatrix.n_cols - 1)++;
      CAPTURE(rotationMatrix);

      CHECK(mant::isRotationMatrix(rotationMatrix) == false);
    }
  }
}

TEST_CASE("isPermutationVector") {
  const arma::uword numberOfPermutations = 2 + getDiscreteRandomNumber();
  CAPTURE(numberOfPermutations);

  const arma::uword numberOfElements = numberOfPermutations + getDiscreteRandomNumber();
  CAPTURE(numberOfElements);
    
  arma::Col<arma::uword> permutation = mant::randomPermutationVector(numberOfElements, numberOfPermutations);
  CAPTURE(permutation);
    
  SECTION("Returns true, if the vector is a permutation vector.") {
    CHECK(mant::isPermutationVector(permutation, numberOfPermutations, numberOfElements) == true);
  }

  SECTION("Returns false, if the vector is not a permutation vector.") {
    SECTION("Returns false, if the number of permutations is larger than the number of elements.") {
      CHECK(mant::isPermutationVector(permutation, numberOfElements + numberOfPermutations, numberOfElements) == false);
    }

    SECTION("Returns false, if the number of permutations is unequal to the number of elements within the vector.") {
      permutation.resize(numberOfPermutations - 1);
      CAPTURE(permutation);

      CHECK(mant::isPermutationVector(permutation, numberOfPermutations, numberOfElements) == false);
    }

    SECTION("Returns false, if any element of the vector is not within [0, numberOfElements - 1].") {
      permutation(0) = numberOfElements;
      CAPTURE(permutation);

      CHECK(mant::isPermutationVector(permutation, numberOfPermutations, numberOfElements) == false);
    }

    SECTION("Returns false, if the elements in the vector are not unique.") {
      permutation(0) = permutation(1);
      CAPTURE(permutation);

      CHECK(mant::isPermutationVector(permutation, numberOfPermutations, numberOfElements) == false);
    }
  }
}

TEST_CASE("isSymmetric") {
  // Some tests expect that the matrix has at least 2 columns/rows.
  const arma::uword numberOfDimensions = 1 + getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  arma::Mat<double> symmetricMatrix = arma::symmatu(getContinuousRandomNumbers(numberOfDimensions, numberOfDimensions));
  CAPTURE(symmetricMatrix);
  
  SECTION("Return true, if the matrix is symmetric.") {
    CHECK(mant::isSymmetric(symmetricMatrix) == true);
  }
  
  SECTION("Returns false, if the matrix is not symmetric.") {
    SECTION("Return false, if the matrix is not square.") {
      symmetricMatrix.shed_col(0);
      CAPTURE(symmetricMatrix);
      
      CHECK(mant::isSymmetric(symmetricMatrix) == false);
    }
    
    SECTION("Return false, if any (i,j)-te element is unequal to the (j,i)-te element.") {
      symmetricMatrix(0, 1)++;
      CAPTURE(symmetricMatrix);
      
      CHECK(mant::isSymmetric(symmetricMatrix) == false);
    }
  }
}

TEST_CASE("isPositiveSemiDefinite") {
  // Some tests expect that the matrix has at least 2 columns/rows.
  const arma::uword numberOfDimensions = 1 + getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  arma::Mat<double> positiveSemiDefinite = getContinuousRandomNumbers(numberOfDimensions, numberOfDimensions);
  positiveSemiDefinite *= positiveSemiDefinite.t();
  CAPTURE(positiveSemiDefinite);
  
  SECTION("Return true, if the matrix is positive semi-definite.") {
    CHECK(mant::isPositiveSemiDefinite(positiveSemiDefinite) == true);
  }
  
  SECTION("Returns false, if the matrix is not positive semi-definite.") {
    SECTION("Return false, if the matrix is not square.") {
      positiveSemiDefinite.shed_col(0);
      CAPTURE(positiveSemiDefinite);
      
      CHECK(mant::isPositiveSemiDefinite(positiveSemiDefinite) == false);
    }
    
    SECTION("Return false, if any eigenvalue is negative or has an imaginary part.") {
      CHECK(mant::isPositiveSemiDefinite(-positiveSemiDefinite) == false);
    }
  }
}

TEST_CASE("isDimensionallyConsistent") {
  SECTION("Returns true, if the number of dimensions is consistent over all samples.") {
    // Explicitly enables the cache, just to be sure.
    mant::cacheSamples = true;
      
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
    
    mant::RandomSearch optimisationAlgorithm;
    optimisationAlgorithm.setMaximalNumberOfIterations(100);
  
    CAPTURE(numberOfDimensions);
    mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);
    CAPTURE(numberOfDimensions);
    optimisationAlgorithm.optimise(optimisationProblem);
    CAPTURE(numberOfDimensions);

    const auto& samples = optimisationProblem.getCachedSamples();
    CAPTURE(numberOfDimensions);
    // We would like to capture the samples, but Catch cannot capture unordered maps.

    CAPTURE(numberOfDimensions);
    CHECK(samples.size() > 0);
    CHECK(mant::isDimensionallyConsistent(samples) == true);
    CAPTURE(numberOfDimensions);
  }

  SECTION("Returns false, if the number of dimensions is inconsistent between any two samples.") {
    // Explicitly enables the cache, just to be sure.
    mant::cacheSamples = true;
      
    mant::RandomSearch optimisationAlgorithm;
    optimisationAlgorithm.setMaximalNumberOfIterations(100);

    // Generate an inconsistent set of samples by concatenating two cached sampling sets of different dimensions.
    // The first optimisation problem
    const arma::uword firstNumberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(firstNumberOfDimensions);

    mant::bbob::SphereFunction firstOptimisationProblem(firstNumberOfDimensions);
    optimisationAlgorithm.optimise(firstOptimisationProblem);
    const auto& firstSamples = firstOptimisationProblem.getCachedSamples();
    CHECK(firstSamples.size() > 0);

    // The second one
    const arma::uword secondNumberOfDimensions = getDifferentDiscreteRandomNumber(firstNumberOfDimensions);
    CAPTURE(secondNumberOfDimensions);

    mant::bbob::SphereFunction secondOptimisationProblem(secondNumberOfDimensions);
    optimisationAlgorithm.optimise(secondOptimisationProblem);
    const auto& secondSamples = secondOptimisationProblem.getCachedSamples();
    CHECK(secondSamples.size() > 0);

    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
    samples.insert(firstSamples.begin(), firstSamples.end());
    samples.insert(secondSamples.begin(), secondSamples.end());
    // We would like to capture the samples, but Catch cannot capture unordered maps.

    CHECK(samples() > 0);
    CHECK(mant::isDimensionallyConsistent(samples) == false);
  }
}
