// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::SchwefelFunction.getObjectiveFunctions", "[bbob::SchwefelFunction][bbob::SchwefelFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      mant::bbob::SchwefelFunction optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(1066.7601145382));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::SchwefelFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Schwefel Function (f20)");
  }
}
