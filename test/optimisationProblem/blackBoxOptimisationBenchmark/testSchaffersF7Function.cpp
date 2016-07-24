// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestSchaffersF7Function : public mant::bbob::SchaffersF7Function {
 public:
  using mant::bbob::SchaffersF7Function::SchaffersF7Function;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::SchaffersF7Function::rotationQ_;
};

SCENARIO("bbob::SchaffersF7Function.getObjectiveFunctions", "[bbob::SchaffersF7Function][bbob::SchaffersF7Function.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestSchaffersF7Function optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(27.7170923412));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::SchaffersF7Function optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Schaffers F7 Function (f17)");
  }
}
