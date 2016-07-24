// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("PopulationBasedOptimisationAlgorithm.setPopulationSize", "[PopulationBasedOptimisationAlgorithm][PopulationBasedOptimisationAlgorithm.setPopulationSize]") {
  GIVEN("A population size") {
    WHEN("The population size is greater than 0") {
      THEN("Throw no exception") {
        mant::PopulationBasedOptimisationAlgorithm optimisationAlgorithm;
        CHECK_NOTHROW(optimisationAlgorithm.setPopulationSize(1));
      }
    }
  }
}

SCENARIO("PopulationBasedOptimisationAlgorithm.getPopulationSize", "[PopulationBasedOptimisationAlgorithm][PopulationBasedOptimisationAlgorithm.getPopulationSize]") {
  WHEN("The default population size is unchanged") {
    THEN("Return 40") {
      mant::PopulationBasedOptimisationAlgorithm optimisationAlgorithm;
      CHECK(optimisationAlgorithm.getPopulationSize() == 40);
    }
  }
}