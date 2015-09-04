// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("GridSearch") {
  SECTION(".optimise") {
    SECTION("Checking the procedure with the default number of samples per dimension.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
      optimisationProblem->setLowerBounds(arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 200 - 100);
      optimisationProblem->setUpperBounds(optimisationProblem->getLowerBounds() + arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 100 + 0.01);
      
      mant::recordSamples = true;
      mant::GridSearch gridSearch(optimisationProblem);
      gridSearch.optimise();
      mant::recordSamples = false;

      std::vector<std::pair<arma::Col<double>, double>> actualSamples = gridSearch.getSamplingProgress();
      
      std::vector<arma::Col<double>> expectedSamples;
      for (const auto& firstParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(0), optimisationProblem->getUpperBounds()(0), 10)) {
        for (const auto& secondParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(1), optimisationProblem->getUpperBounds()(1), 10)) {
          expectedSamples.push_back({firstParameter, secondParameter});
        }
      }
      
      COMPARE_SET(actualSamples, expectedSamples);
    }
    
    
    SECTION("Checking the procedure with a parametrised number of samples per dimension.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(5));
      optimisationProblem->setLowerBounds(arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 200 - 100);
      optimisationProblem->setUpperBounds(optimisationProblem->getLowerBounds() + arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 100 + 0.01);
      
      mant::recordSamples = true;
      mant::GridSearch gridSearch(optimisationProblem);
      arma::Col<arma::uword> numberOfSamplesPerDimension = arma::randi<arma::Col<arma::uword>>(optimisationProblem->numberOfDimensions_, arma::distr_param(2, 10));
      gridSearch.setNumberOfSamplesPerDimension(numberOfSamplesPerDimension);
      gridSearch.setMaximalNumberOfIterations(arma::prod(numberOfSamplesPerDimension));
      gridSearch.optimise();
      mant::recordSamples = false;

      std::vector<std::pair<arma::Col<double>, double>> actualSamples = gridSearch.getSamplingProgress();
      
      std::vector<arma::Col<double>> expectedSamples;
      for (const auto& firstParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(0), optimisationProblem->getUpperBounds()(0), numberOfSamplesPerDimension(0))) {
        for (const auto& secondParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(1), optimisationProblem->getUpperBounds()(1), numberOfSamplesPerDimension(1))) {
          for (const auto& thirdParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(2), optimisationProblem->getUpperBounds()(2), numberOfSamplesPerDimension(2))) {
            for (const auto& fourthParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(3), optimisationProblem->getUpperBounds()(3), numberOfSamplesPerDimension(3))) {
              for (const auto& fifthParameter : arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(4), optimisationProblem->getUpperBounds()(4), numberOfSamplesPerDimension(4))) {
                expectedSamples.push_back({firstParameter, secondParameter, thirdParameter, fourthParameter, fifthParameter});
              }
            }
          }
        }
      }
      
      COMPARE_SET(actualSamples, expectedSamples);
    }
  }

  SECTION("Exception tests") {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
    mant::GridSearch gridSearch(optimisationProblem);

    SECTION("Throws an exception, if the number of iterations is lower then number of samples.") {
      gridSearch.setNumberOfSamplesPerDimension({100, 100});
      gridSearch.setMaximalNumberOfIterations(10);
      CHECK_THROWS_AS(gridSearch.optimise(), std::logic_error);
    }

    SECTION("Throws an exception, if the size of samples is not equal to the number of dimension of the problem") {
      CHECK_THROWS_AS(gridSearch.setNumberOfSamplesPerDimension({100, 100, 100}), std::logic_error);
      CHECK_THROWS_AS(gridSearch.setNumberOfSamplesPerDimension({100}), std::logic_error);
    }

    SECTION("Throws an exception, if the any number of samples per dimensions is lower than 2") {
      CHECK_THROWS_AS(gridSearch.setNumberOfSamplesPerDimension({1, 10}), std::logic_error);
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
      CHECK(mant::GridSearch(optimisationProblem).toString() == "grid_search");
    }
  }
}
