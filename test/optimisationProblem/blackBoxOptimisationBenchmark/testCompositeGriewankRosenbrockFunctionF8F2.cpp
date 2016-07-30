// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::CompositeGriewankRosenbrockFunctionF8F2.getObjectiveFunctions", "[bbob::CompositeGriewankRosenbrockFunctionF8F2][bbob::CompositeGriewankRosenbrockFunctionF8F2.getObjectiveFunctions]") {
  GIVEN("A parameters") {
    THEN("Return its objective value") {
      mant::bbob::CompositeGriewankRosenbrockFunctionF8F2 optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(5.7346656627));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::CompositeGriewankRosenbrockFunctionF8F2 optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Composite Griewank Rosenbrock Function F8F2 (f19)");
  }
}
