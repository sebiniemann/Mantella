// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestStepEllipsoidalFunction : public mant::bbob::StepEllipsoidalFunction {
 public:
  using mant::bbob::StepEllipsoidalFunction::StepEllipsoidalFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::StepEllipsoidalFunction::rotationQ_;
};

SCENARIO("bbob::StepEllipsoidalFunction.getObjectiveFunctions", "[bbob::StepEllipsoidalFunction][bbob::StepEllipsoidalFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestStepEllipsoidalFunction optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(686.7543730745));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::StepEllipsoidalFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Step Ellipsoidal Function (f7)");
  }
}
