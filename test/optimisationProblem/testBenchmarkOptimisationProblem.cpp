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

class TestBenchmarkOptimisationProblem : public mant::BenchmarkOptimisationProblem {
  public:
    using mant::BenchmarkOptimisationProblem::BenchmarkOptimisationProblem;
    
    // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
    double getBestObjectiveValue() const override {
      return objectiveValueTranslation_;
    }
};

TEST_CASE("BenchmarkOptimisationProblem") {
  arma::uword numberOfDimensions = getDiscreteRandomNumber();
  CAPTURE(numberOfDimensions);
  
  TestBenchmarkOptimisationProblem optimisationProblem(numberOfDimensions);
  
  SECTION(".getBestObjectiveValue") {
    SECTION("Returns the best (reachable) objective value.") {
      const double& objectiveValueTranslation = getContinuousRandomNumber();
      CAPTURE(objectiveValueTranslation);
      optimisationProblem.setObjectiveValueTranslation(objectiveValueTranslation);
      
      CHECK(optimisationProblem.getBestObjectiveValue() == Approx(objectiveValueTranslation));
    }
  }
}
