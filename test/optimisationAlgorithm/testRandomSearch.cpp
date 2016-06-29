// Catch
#include <catch.hpp>
#include "../catchHelper.hpp"

SCENARIO("RandomSearch.optimise", "[RandomSearch][RandomSearch.optimise]") {
  GIVEN("An optimisation problem") {
    THEN("Optimise the problem") {
      mant::bbob::SphereFunction optimisationProblem(3);
      optimisationProblem.setObjectiveValueTranslation(0.0);

      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(10000);
      optimisationAlgorithm.setAcceptableObjectiveValue(1e-1);

      arma::vec medianUsedNumberOfIterations(100);
      for (arma::uword n = 0; n < medianUsedNumberOfIterations.n_elem; ++n) {
        optimisationAlgorithm.optimise(optimisationProblem);
        medianUsedNumberOfIterations(n) = static_cast<double>(optimisationAlgorithm.getUsedNumberOfIterations());
      }

      // The value is obtained from doubling the median used number of iterations based on 10.000 repetitions.
      CHECK(arma::median(medianUsedNumberOfIterations) <= 9000);
    }
  }
}