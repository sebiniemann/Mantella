// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestSchaffersF7FunctionIllConditioned : public mant::bbob::SchaffersF7FunctionIllConditioned {
 public:
  using mant::bbob::SchaffersF7FunctionIllConditioned::
      SchaffersF7FunctionIllConditioned;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::SchaffersF7FunctionIllConditioned::rotationQ_;
};

SCENARIO("bbob::SchaffersF7FunctionIllConditioned.getObjectiveFunctions", "[bbob::SchaffersF7FunctionIllConditioned][bbob::SchaffersF7FunctionIllConditioned.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestSchaffersF7FunctionIllConditioned optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(114.514121272));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::SchaffersF7FunctionIllConditioned optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Schaffers F7 Function, ill-conditioned (f18)");
  }
}
