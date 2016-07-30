// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestLunacekBiRastriginFunction : public mant::bbob::LunacekBiRastriginFunction {
 public:
  using mant::bbob::LunacekBiRastriginFunction::LunacekBiRastriginFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::LunacekBiRastriginFunction::rotationR_;
  using mant::bbob::LunacekBiRastriginFunction::rotationQ_;
};

SCENARIO("bbob::LunacekBiRastriginFunction.getObjectiveFunctions", "[bbob::LunacekBiRastriginFunction][bbob::LunacekBiRastriginFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestLunacekBiRastriginFunction optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);
      optimisationProblem.rotationR_ = mant::rotationMatrix3d(-0.1, -0.2, -0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(60.6314611883));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::LunacekBiRastriginFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Lunacek bi-Rastrigin Function (f24)");
  }
}
