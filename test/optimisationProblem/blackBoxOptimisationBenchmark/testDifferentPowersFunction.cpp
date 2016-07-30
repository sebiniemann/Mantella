// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::DifferentPowersFunction.getObjectiveFunctions", "[bbob::DifferentPowersFunction][bbob::DifferentPowersFunction.getObjectiveFunctions]") {
  GIVEN("A parameters") {
    THEN("Return its objective value") {
      mant::bbob::DifferentPowersFunction optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(27.3130005675));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::DifferentPowersFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Different Powers Function (f14)");
  }
}
