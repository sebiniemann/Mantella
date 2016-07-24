// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestRastriginFunctionRotated : public mant::bbob::RastriginFunctionRotated {
 public:
  using mant::bbob::RastriginFunctionRotated::RastriginFunctionRotated;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::RastriginFunctionRotated::rotationR_;
  using mant::bbob::RastriginFunctionRotated::rotationQ_;
};

SCENARIO("bbob::RastriginFunctionRotated.getObjectiveFunctions", "[bbob::RastriginFunctionRotated][bbob::RastriginFunctionRotated.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestRastriginFunctionRotated optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);
      optimisationProblem.rotationR_ = mant::rotationMatrix3d(-0.1, -0.2, -0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(253.0773451408));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::RastriginFunctionRotated optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Rastrigin Function, rotated (f15)");
  }
}
