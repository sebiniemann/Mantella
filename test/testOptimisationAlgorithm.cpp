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
    
    optimisationAlgorithm.setMaximalNumberOfIterations(1000);
    optimisationAlgorithm.optimise(optimisationProblem, arma::Mat<double>());
    
    
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
        CHECK_THROWS_AS(optimisationAlgorithm.setBoundaryHandlingFunction(nullptr), std::logic_error);
      }
    }
  }
  
  SECTION(".setIsDegeneratedFunction") {
    // The influence on *.optimise* is already tested in *SECTION(".optimise")*.
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationAlgorithm.setNextParametersFunction(nullptr), std::logic_error);
      }
    }
  }
  
  SECTION(".setDegenerationHandlingFunction") {
    // The influence on *.optimise* is already tested in *SECTION(".optimise")*.
    
    SECTION("Exception tests:") {
      SECTION("Throws an exception, if no callable function is set.") {
        CHECK_THROWS_AS(optimisationAlgorithm.setDegenerationHandlingFunction(nullptr), std::logic_error);
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
      SECTION("Throw an exception, if the maximal duration is infinite.") {
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
      mant::recordSamplingHistory = false;
      
      arma::uword numberOfDimensions = getDiscreteRandomNumber();
      CAPTURE(numberOfDimensions);
      
      mant::OptimisationProblem optimisationProblem(numberOfDimensions);
      optimisationProblem.setObjectiveFunction([] (
          const arma::Col<double>& parameter) {
        return arma::accu(parameter % mant::range<double>(1, parameter.n_elem));
      });
        
      optimisationAlgorithm.optimise(optimisationProblem, arma::Mat<double>());
      
      CHECK(optimisationAlgorithm.getSamplingHistory().size() == 0);
    }
  }
  
  SECTION(".reset") {
  
  }
}
