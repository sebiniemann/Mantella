// Catch
#include <catch.hpp>

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
  SECTION("Throws std::logic_error if the expression is false.") {
    CHECK_THROWS_AS(mant::verify(false, "Some message ..."), std::logic_error);
  }
  
  SECTION("Does nothing if the expression is true.") {
    CHECK_NOTHROW(mant::verify(true, "Some message ..."));
  }
}

TEST_CASE("isRotationMatrix") {
  SECTION("Returns true if a rotation matrix is provided.") {
    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(2, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);
    
    const arma::Mat<double>& rotationMatrix = mant::getRandomRotationMatrix(numberOfDimensions);
    CAPTURE(rotationMatrix);
    
    CHECK(mant::isRotationMatrix(rotationMatrix) == true);
  }
  
  SECTION("Returns false if the provided matrix is not a rotation matrix.") {
    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(2, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);
    
    arma::Mat<double> nonRotationMatrix = mant::getRandomRotationMatrix(numberOfDimensions);
    
    SECTION("Returns false if the provided matrix is not square.") {
      nonRotationMatrix.shed_col(0);
      CAPTURE(nonRotationMatrix);
    
      CHECK(mant::isRotationMatrix(nonRotationMatrix) == false);
    }
    
    SECTION("Returns false if the provided matrix has not an determinant of (nearly) -1 or 1.") {
      // Increasing any element should change the determinant significantly
      nonRotationMatrix(0, 0) += 1;
      CAPTURE(nonRotationMatrix);
      
      CHECK(mant::isRotationMatrix(nonRotationMatrix) == false);
    }
    
    SECTION("Returns false if its transpose is not (nearly) equal to its inverse.") {
      // Increasing any element should divert its transpose and inverse significantly
      nonRotationMatrix(0, nonRotationMatrix.n_cols - 1) += 1;
      CAPTURE(nonRotationMatrix);
    
      CHECK(mant::isRotationMatrix(nonRotationMatrix) == false);
    }
  }
}

TEST_CASE("isPermutation") {
  SECTION("Returns true if a permutation is provided.") {
    const arma::uword numberOfPermutations = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfPermutations);
    
    const arma::uword numberOfElements = numberOfPermutations + std::uniform_int_distribution<arma::uword>(0, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfElements);
    
    const arma::Col<arma::uword>& permutation = mant::getRandomPermutation(numberOfElements, numberOfPermutations);
    CAPTURE(permutation);
    
    CHECK(mant::isPermutation(permutation, numberOfPermutations, numberOfElements) == true);
  }
  
  SECTION("Returns false if the provided vector is not a permutation.") {
    const arma::uword numberOfPermutations = std::uniform_int_distribution<arma::uword>(2, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfPermutations);
    
    const arma::uword numberOfElements = numberOfPermutations + std::uniform_int_distribution<arma::uword>(0, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfElements);
    
    arma::Col<arma::uword> nonPermutation = mant::getRandomPermutation(numberOfElements, numberOfPermutations);
    
    SECTION("Returns false if the number of permutations is larger than the number of elements.") {
      nonPermutation.resize(numberOfElements + 1);
      CAPTURE(nonPermutation);
      
      CHECK(mant::isPermutation(nonPermutation, numberOfPermutations, numberOfElements) == false);
    }
    
    SECTION("Returns false if the number of permutations is not equal to the number of elements within the vector.") {
      nonPermutation.resize(numberOfPermutations - 1);
      CAPTURE(nonPermutation);
      
      CHECK(mant::isPermutation(nonPermutation, numberOfPermutations, numberOfElements) == false);
    }
    
    SECTION("Returns false if any element of the vector is not within [0, numberOfElements - 1]") {
      nonPermutation(0) = numberOfElements;
      CAPTURE(nonPermutation);
      
      CHECK(mant::isPermutation(nonPermutation, numberOfPermutations, numberOfElements) == false);
    }
    
    SECTION("Returns false if the vector is not unique") {
      nonPermutation(0) = nonPermutation(1);
      CAPTURE(nonPermutation);
      
      CHECK(mant::isPermutation(nonPermutation, numberOfPermutations, numberOfElements) == false);
    }
  }
}

TEST_CASE("isDimensionallyConsistent") {
  SECTION("Returns true if the number of dimensions is consistent over all samples.") {
    const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
    CAPTURE(numberOfDimensions);
    
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(numberOfDimensions));
    mant::RandomSearch optimisationAlgorithm(optimisationProblem);
    optimisationAlgorithm.optimise();
    
    const auto& samples = optimisationProblem->getCachedSamples();
    // unordered maps cannot be captured by Catch
    
    CHECK(mant::isDimensionallyConsistent(samples) == true);
  }
  
  SECTION("Returns false if the number of dimensions is inconsistent between any two samples.") {
    std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
    
    const arma::uword firstNumberOfDimensions = std::uniform_int_distribution<arma::uword>(2, 10)(mant::Rng::getGenerator());
    CAPTURE(firstNumberOfDimensions);
    
    std::shared_ptr<mant::OptimisationProblem> firstOptimisationProblem(new mant::bbob::SphereFunction(firstNumberOfDimensions));
    mant::RandomSearch firstOptimisationAlgorithm(firstOptimisationProblem);
    firstOptimisationAlgorithm.optimise();
    
    for (const auto& sample : firstOptimisationProblem->getCachedSamples()) {
      samples.insert(sample);
    }
    
    const arma::uword secondNumberOfDimensions = std::uniform_int_distribution<arma::uword>(1, firstNumberOfDimensions - 1)(mant::Rng::getGenerator());
    CAPTURE(secondNumberOfDimensions);
    
    std::shared_ptr<mant::OptimisationProblem> secondOptimisationProblem(new mant::bbob::SphereFunction(secondNumberOfDimensions));
    mant::RandomSearch secondOptimisationAlgorithm(secondOptimisationProblem);
    secondOptimisationAlgorithm.optimise();
    
    // Hopefully, we wont get a hash collision, otherwise mant::IsEqual may throw an exception when comparing vectors with an unequal number of dimensions.
    for (const auto& sample : secondOptimisationProblem->getCachedSamples()) {
      samples.insert(sample);
    }
   
    // unordered maps cannot be captured by Catch
    
    CHECK(mant::isDimensionallyConsistent(samples) == true);
  }
}
