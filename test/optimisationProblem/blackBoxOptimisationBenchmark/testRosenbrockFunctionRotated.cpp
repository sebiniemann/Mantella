// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::RosenbrockFunctionRotated.getObjectiveFunctions", "[bbob::RosenbrockFunctionRotated][bbob::RosenbrockFunctionRotated.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      mant::bbob::RosenbrockFunctionRotated optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(1569.0));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::RosenbrockFunctionRotated optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Rosenbrock Function, rotated (f9)");
  }
}
