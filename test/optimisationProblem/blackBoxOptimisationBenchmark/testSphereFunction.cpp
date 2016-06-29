// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::SphereFunction.getObjectiveFunctions", "[bbob::SphereFunction][bbob::SphereFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      mant::bbob::SphereFunction optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(14.0));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::SphereFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Sphere Function (f1)");
  }
}
