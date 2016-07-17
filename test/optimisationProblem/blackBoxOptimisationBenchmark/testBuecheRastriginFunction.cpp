// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

SCENARIO("bbob::BuecheRastriginFunction.getObjectiveFunctions", "[bbob::BuecheRastriginFunction][bbob::BuecheRastriginFunction.getObjectiveFunctions]") {
  GIVEN("A parameters") {
    THEN("Return its objective value") {
      mant::bbob::BuecheRastriginFunction optimisationProblem(3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(8869.6557599233));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::BuecheRastriginFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Büche-Rastrigin Function (f4)");
  }
}
