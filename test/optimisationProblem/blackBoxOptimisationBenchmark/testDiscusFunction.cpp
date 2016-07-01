// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::DiscusFunction.getObjectiveFunctions", "[bbob::DiscusFunction][bbob::DiscusFunction.getObjectiveFunctions]") {
  GIVEN("A parameters") {
    THEN("Return its objective value") {
      mant::bbob::DiscusFunction optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(1000012.8064966329));
    }
  }
  THEN("Return the objective function name") {
    mant::bbob::DiscusFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Discus Function (f11)");
  }
}
