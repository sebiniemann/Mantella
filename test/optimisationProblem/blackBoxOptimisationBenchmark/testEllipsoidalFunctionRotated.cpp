// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::EllipsoidalFunctionRotated.getObjectiveFunctions", "[bbob::EllipsoidalFunctionRotated][bbob::EllipsoidalFunctionRotated.getObjectiveFunctions]") {
  GIVEN("A parameters") {
    THEN("Return its objective value") {
      mant::bbob::EllipsoidalFunctionRotated optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(8724996.1974065471));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::EllipsoidalFunctionRotated optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Ellipsoidal Function, rotated (f10)");
  }
}
