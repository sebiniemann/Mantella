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

TEST_CASE("BenchmarkOptimisationProblem") {
  arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  mant::BenchmarkOptimisationProblem optimisationProblem(numberOfDimensions);
  
  SECTION(".setAcceptableObjectiveValuePrecision") {
    SECTION("Updates the acceptable objective value precision, relative to the objective value translation.") {
      const double& objectiveValueTranslation = getContinuousRandomNumber();
      CAPTURE(objectiveValueTranslation);
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
    
      const double acceptableObjectiveValuePrecision = std::abs(getContinuousRandomNumber());
      CAPTURE(acceptableObjectiveValuePrecision);
      
      optimisationProblem.setAcceptableObjectiveValuePrecision(acceptableObjectiveValuePrecision);
      
      CHECK(optimisationProblem.getAcceptableObjectiveValuePrecision() == Approx(acceptableObjectiveValuePrecision));
      CHECK(optimisationProblem.getAcceptableObjectiveValueThreshold() == Approx(objectiveValueTranslation + acceptableObjectiveValuePrecision));
    }
    
    SECTION("Works with infinite values.") {
      CHECK_NOTHROW(optimisationProblem.setAcceptableObjectiveValuePrecision(arma::datum::inf));
    }
    
    SECTION("Does not reset the cache and counters.") {
      // Explicitly enables the cache, just to be sure.
      mant::cacheSamples = true;
      
      auto objectiveFunction = [] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      };
      optimisationProblem.setObjectiveFunction(objectiveFunction);
        
      const arma::uword numberOfParameters = getDiscreteRandomNumber();
      CAPTURE(numberOfParameters);
      arma::Mat<double> parameters = getContinuousRandomNumbers(numberOfDimensions, numberOfParameters);
      CAPTURE(parameters);
      
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        // Populates the cache and increases the counters.
        optimisationProblem.getObjectiveValue(parameters.col(n));
      }
      
      const double acceptableObjectiveValuePrecision = std::abs(getContinuousRandomNumber());
      CAPTURE(acceptableObjectiveValuePrecision);
      optimisationProblem.setAcceptableObjectiveValuePrecision(acceptableObjectiveValuePrecision);
    
      CHECK(optimisationProblem.getCachedSamples().size() != 0);
      CHECK(optimisationProblem.getNumberOfEvaluations() != 0);
      CHECK(optimisationProblem.getNumberOfDistinctEvaluations() != 0);
    }
    
#if defined(SUPPORT_MPI)
    SECTION("Synchronises the parametrisation over MPI.") {
      double acceptableObjectiveValuePrecision = std::abs(getContinuousRandomNumber());
      CAPTURE(acceptableObjectiveValuePrecision);
      
      optimisationProblem.setAcceptableObjectiveValuePrecision(acceptableObjectiveValuePrecision);
      MPI_Bcast(&acceptableObjectiveValuePrecision, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      
      CHECK(optimisationProblem.getAcceptableObjectiveValuePrecision() == Approx(acceptableObjectiveValuePrecision));
    }
#endif

    SECTION("Exception tests:") {
      SECTION("Throw an exception, if the parameter is negative.") {
        const double acceptableObjectiveValuePrecision = -std::abs(getContinuousRandomNumber());
        CAPTURE(acceptableObjectiveValuePrecision);
        
        CHECK_THROWS_AS(optimisationProblem.setAcceptableObjectiveValuePrecision(acceptableObjectiveValuePrecision), std::logic_error);
      }
    }
  }
  
  SECTION(".getAcceptableObjectiveValuePrecision") {
    // This is already covered by *SECTION(".setAcceptableObjectiveValuePrecision")*.
  }
}
