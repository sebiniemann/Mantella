// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::LinearSlope.getObjectiveFunctions", "[bbob::LinearSlope][bbob::LinearSlope.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      mant::bbob::LinearSlope optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(46.1359436212));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::LinearSlope optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Linear Slope (f5)");
  }
}
