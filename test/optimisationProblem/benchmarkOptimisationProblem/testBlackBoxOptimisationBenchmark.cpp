// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <limits>
#include <map>
#include <algorithm>
#include <array>

// Mantella
#include <mantella>

TEST_CASE("BlackBoxOptimisationBenchmark") {
  // Some tests have only pre-calculated result for a 5-dimensional optimisation problem.
  arma::uword numberOfDimensions = 5;
  CAPTURE(numberOfDimensions);
  
  mant::bbob::BlackBoxOptimisationBenchmark optimisationProblem(numberOfDimensions);

  SECTION("Sets the lower and upper bounds as well as the acceptable objective value precision.") {
    IS_EQUAL(optimisationProblem.getLowerBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) - 5);
    IS_EQUAL(optimisationProblem.getUpperBounds(), arma::zeros<arma::Col<double>>(numberOfDimensions) + 5);
    
    const double& objectiveValueTranslation = SYNCRONISED(getContinuousRandomNumber());
    CAPTURE(objectiveValueTranslation);
    
    optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
    CHECK(optimisationProblem.getOptimalObjectiveValue() == Approx(objectiveValueTranslation));
  }
  
  SECTION("Randomises the objective value translation within [-1000, 1000], using a Cauchy distribution with location 0.0 and scale 100.0") {
    // TODO
    // Generate 10.000 parameter, initialise the problem 10.000 times and get the translation indirectly from *.getObjectiveValue*.
    // Compare the difference between the parameter and the PDF (Cauchy), assuming that the error should fulfil *IS_UNIFORM*.
  }
}
