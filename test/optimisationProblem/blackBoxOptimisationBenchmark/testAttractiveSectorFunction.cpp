// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestAttractiveSectorFunction : public mant::bbob::AttractiveSectorFunction {
 public:
  using mant::bbob::AttractiveSectorFunction::AttractiveSectorFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::AttractiveSectorFunction::parameterTranslation_;
  using mant::bbob::AttractiveSectorFunction::rotationQ_;
};

SCENARIO("bbob::AttractiveSectorFunction.getObjectiveFunctions", "[bbob::AttractiveSectorFunction][bbob::AttractiveSectorFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestAttractiveSectorFunction optimisationProblem(3);
      optimisationProblem.parameterTranslation_ = arma::vec({2.0, 3.0, -1.0});
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(49576.1198225102));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::AttractiveSectorFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Attractive Sector Function (f6)");
  }
}
