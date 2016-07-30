// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestBentCigarFunction : public mant::bbob::BentCigarFunction {
 public:
  using mant::bbob::BentCigarFunction::BentCigarFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::BentCigarFunction::rotationQ_;
};

SCENARIO("bbob::BentCigarFunction.getObjectiveFunctions", "[bbob::BentCigarFunction][bbob::BentCigarFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestBentCigarFunction optimisationProblem(3);
      optimisationProblem.rotationQ_ = mant::rotationMatrix3d(0.1, 0.2, 0.3);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0, 3.0}) == Approx(22424232.8493554294));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::BentCigarFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Bent Cigar Function (f12)");
  }
}
