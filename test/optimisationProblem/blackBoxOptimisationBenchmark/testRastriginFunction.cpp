// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::RastriginFunction.getObjectiveFunctions", "[bbob::RastriginFunction][bbob::RastriginFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      mant::bbob::RastriginFunction optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(235.3181057912));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::RastriginFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Rastrigin Function (f3)");
  }
}
