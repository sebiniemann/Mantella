// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestWeierstrassFunction : public mant::bbob::WeierstrassFunction {
 public:
  using mant::bbob::WeierstrassFunction::WeierstrassFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::WeierstrassFunction::rotationQ_;
  using mant::bbob::WeierstrassFunction::rotationR_;
};

SCENARIO("bbob::WeierstrassFunction.getObjectiveFunctions", "[bbob::WeierstrassFunction][bbob::WeierstrassFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestWeierstrassFunction optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);
      optimisationProblem.rotationR_ = mant::rotationMatrix3d(-0.1, -0.2, -0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(138.9277862739));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::WeierstrassFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Weierstrass Function (f16)");
  }
}
