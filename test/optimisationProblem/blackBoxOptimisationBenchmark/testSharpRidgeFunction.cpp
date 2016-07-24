// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestSharpRidgeFunction : public mant::bbob::SharpRidgeFunction {
 public:
  using mant::bbob::SharpRidgeFunction::SharpRidgeFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::SharpRidgeFunction::rotationQ_;
};

SCENARIO("bbob::SharpRidgeFunction.getObjectiveFunctions", "[bbob::SharpRidgeFunction][bbob::SharpRidgeFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestSharpRidgeFunction optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(952.6911659767));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::SharpRidgeFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Sharp Ridge Function (f13)");
  }
}
