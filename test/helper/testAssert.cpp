// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <random>

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
  const arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  arma::Mat<double> rotationMatrix = mant::getRandomRotationMatrix(numberOfDimensions);
  CAPTURE(rotationMatrix);
    
  SECTION("Returns true, if a rotation matrix is provided.") {
    CHECK(mant::isRotationMatrix(rotationMatrix) == true);
  }

  SECTION("Returns false, if the provided matrix is not a rotation matrix.") {
    SECTION("Returns false if the provided matrix is not square.") {
      rotationMatrix.shed_col(0);
      CAPTURE(rotationMatrix);

      CHECK(mant::isRotationMatrix(rotationMatrix) == false);
    }

    SECTION("Returns false, if the provided matrix has not an determinant of (nearly) -1 or 1.") {
      // Assuming the the matrix is normalised, increasing a single element be 1 should change the determinant significantly.
      rotationMatrix(0, 0) += 1;
      CAPTURE(rotationMatrix);

      CHECK(mant::isRotationMatrix(rotationMatrix) == false);
    }

    SECTION("Returns false, if its transpose is not (nearly) equal to its inverse.") {
      // Assuming the the matrix is normalised, increasing any element should divert its transpose and inverse significantly.
      rotationMatrix(0, rotationMatrix.n_cols - 1) += 1;
      CAPTURE(rotationMatrix);

      CHECK(mant::isRotationMatrix(rotationMatrix) == false);
    }
  }
}

TEST_CASE("isPermutation") {
  const arma::uword numberOfPermutations = 2 + getDiscreteRandomNumber();
  CAPTURE(numberOfPermutations);

  const arma::uword numberOfElements = numberOfPermutations + getDiscreteRandomNumber();
  CAPTURE(numberOfElements);
    
  arma::Col<arma::uword> permutation = mant::getRandomPermutation(numberOfElements, numberOfPermutations);
  CAPTURE(permutation);
    
  SECTION("Returns true, if a permutation is provided.") {
    CHECK(mant::isPermutation(permutation, numberOfPermutations, numberOfElements) == true);
  }

  SECTION("Returns false, if the provided vector is not a permutation.") {
    SECTION("Returns false if the number of permutations is larger than the number of elements.") {
      permutation.resize(numberOfElements + 1);
      CAPTURE(permutation);

      CHECK(mant::isPermutation(permutation, numberOfPermutations, numberOfElements) == false);
    }

    SECTION("Returns false, if the number of permutations is not equal to the number of elements within the vector.") {
      permutation.resize(numberOfPermutations - 1);
      CAPTURE(permutation);

      CHECK(mant::isPermutation(permutation, numberOfPermutations, numberOfElements) == false);
    }

    SECTION("Returns false, if any element of the vector is not within [0, numberOfElements - 1]") {
      permutation(0) = numberOfElements;
      CAPTURE(permutation);

      CHECK(mant::isPermutation(permutation, numberOfPermutations, numberOfElements) == false);
    }

    SECTION("Returns false, if the vector is not unique") {
      permutation(0) = permutation(1);
      CAPTURE(permutation);

      CHECK(mant::isPermutation(permutation, numberOfPermutations, numberOfElements) == false);
    }
  }
}

TEST_CASE("isDimensionallyConsistent") {
  SECTION("Returns true, if the number of dimensions is consistent over all samples.") {
    const arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
  
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();

    const auto& samples = optimisationProblem->getCachedSamples();
    // We liked to log the samples, in case of an error, but Catch cannot capture unordered maps.

    CHECK(mant::isDimensionallyConsistent(samples) == true);
  }

  SECTION("Returns false, if the number of dimensions is inconsistent between any two samples.") {
    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;

    // Generate an inconsistent set of samples by concatenating two cached sampling sets of dimensionally different optimisation problems.
    // The first optimisation problem
    const arma::uword firstNumberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(firstNumberOfDimensions);

    std::shared_ptr<mant::OptimisationProblem> firstOptimisationProblem(new mant::bbob::SphereFunction(firstNumberOfDimensions));
    mant::RandomSearch firstOptimisationAlgorithm(firstOptimisationProblem);
    firstOptimisationAlgorithm.optimise();

    // The second one
    const arma::uword secondNumberOfDimensions = getDifferentDiscreteRandomNumber(firstNumberOfDimensions);
    CAPTURE(secondNumberOfDimensions);

    std::shared_ptr<mant::OptimisationProblem> secondOptimisationProblem(new mant::bbob::SphereFunction(secondNumberOfDimensions));
    mant::RandomSearch secondOptimisationAlgorithm(secondOptimisationProblem);
    secondOptimisationAlgorithm.optimise();

    const auto& firstSamples = firstOptimisationProblem->getCachedSamples();
    samples.insert(firstSamples.begin(), firstSamples.end());
    const auto& secondSamples = secondOptimisationProblem->getCachedSamples();
    samples.insert(secondSamples.begin(), secondSamples.end());
    // We liked to log the samples, in case of an error, but Catch cannot capture unordered maps.

    CHECK(mant::isDimensionallyConsistent(samples) == false);
  }
}
