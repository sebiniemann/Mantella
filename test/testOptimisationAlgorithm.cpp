// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// C++ standard library
#include <memory>

// Mantella
#include <mantella>

TEST_CASE("OptimisationAlgorithm") {
  mant::OptimisationAlgorithm optimisationAlgorithm;
  optimisationAlgorithm.setMaximalNumberOfIterations(100);
  
  SECTION(".optimise") {
    arma::uword numberOfDimensions = getDiscreteRandomNumber();
    CAPTURE(numberOfDimensions);
  
    mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);
    
    optimisationAlgorithm.setNextParametersFunction([] (
        const arma::Mat<double>& parameters,
        const arma::Col<double>& objectiveValues,
        const arma::Col<double>& differences) {
      return arma::randu<arma::Col<double>>(parameters.n_rows);
    });
    optimisationAlgorithm.optimise(optimisationProblem, arma::randu<arma::Col<double>>(numberOfDimensions));
    
    
  }
  
  SECTION(".setNextParametersFunction") {
    // The influence on *.optimise* is already tested in *SECTION(".optimise")*.
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationAlgorithm.setNextParametersFunction(nullptr), std::logic_error);
      }
    }
  }
  
  SECTION(".setBoundaryHandlingFunction") {
    // The influence on *.optimise* is already tested in *SECTION(".optimise")*.
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationAlgorithm.setBoundariesHandlingFunction(nullptr), std::logic_error);
      }
    }
  }
  
  SECTION(".isStagnatingFunction") {
    // The influence on *.optimise* is already tested in *SECTION(".optimise")*.
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationAlgorithm.isStagnatingFunction(nullptr), std::logic_error);
      }
    }
  }
  
  SECTION(".setRestartingFunction") {
    // The influence on *.optimise* is already tested in *SECTION(".optimise")*.
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationAlgorithm.setRestartingFunction(nullptr), std::logic_error);
      }
    }
  }
  
  SECTION(".setAcceptableObjectiveValue") {
    SECTION("Updates the acceptable objective value.") {
      
    }
  }
  
  SECTION(".getAcceptableObjectiveValue") {
    // This is already covered by *SECTION(".setAcceptableObjectiveValue")*.
  }
  
  SECTION(".setMaximalNumberOfIterations") {
    SECTION("Updates the maximal number of iterations.") {
    
    }
  }
  
  SECTION(".getMaximalNumberOfIterations") {
    // This is already covered by *SECTION(".setMaximalNumberOfIterations")*.
  }
  
  SECTION(".setMaximalDuration") {
    SECTION("Updates the maximal duration.") {
    
    }
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if the maximal duration is less than or equal to 0.") {
      
      }
    }
  }
  
  SECTION(".getMaximalDuration") {
    // This is already covered by *SECTION(".setMaximalDuration")*.
  }
  
  SECTION(".isFinished") {
    SECTION("Returns true, if a parameter with an acceptable objective value was found.") {
    
    }
    
    SECTION("Returns false, if no parameter with an acceptable objective value was found.") {
    
    }
  }
  
  SECTION(".isTerminated") {
    SECTION("Returns true, if the maximal number of iteration was reached.") {
    
    }
    
    SECTION("Returns true, if the maximal duration was reached.") {
    
    }
    
    SECTION("Returns false, if neither the maximal number of iterations, nor the maximal duration was reached.") {
    
    }
  }
  
  SECTION(".getSamplingHistory") {
    SECTION("Returns all samples from this optimisation process (in order).") {
    
    }
    
    SECTION("Is empty, if the sample history recording is disabled.") {
      mant::isRecordingSampling = false;
      
      arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);
      
      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);
        
      optimisationAlgorithm.setNextParametersFunction([] (
          const arma::Mat<double>& parameters,
          const arma::Col<double>& objectiveValues,
          const arma::Col<double>& differences) {
        return arma::randu<arma::Col<double>>(parameters.n_rows);
      });
      optimisationAlgorithm.setAcceptableObjectiveValue(optimisationProblem.getOptimalObjectiveValue() + 1e-3);
      optimisationAlgorithm.optimise(optimisationProblem, arma::randu<arma::Col<double>>(numberOfDimensions));
      
      CHECK(optimisationAlgorithm.getSamplingHistory().size() == 0);
    }
  }
  
  SECTION(".reset") {
  
  }
}
