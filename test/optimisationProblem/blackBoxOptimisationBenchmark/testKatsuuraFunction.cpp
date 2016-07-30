// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestKatsuuraFunction : public mant::bbob::KatsuuraFunction {
 public:
  using mant::bbob::KatsuuraFunction::KatsuuraFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::KatsuuraFunction::rotationQ_;
};

SCENARIO("bbob::KatsuuraFunction.getObjectiveFunctions", "[bbob::KatsuuraFunction][bbob::KatsuuraFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestKatsuuraFunction optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(24.0317128026));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::KatsuuraFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Katsuura Function (f23)");
  }
}
