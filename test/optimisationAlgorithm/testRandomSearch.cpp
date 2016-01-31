// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

class TestRandomSearch : public mant::RandomSearch {
 public:
  using mant::RandomSearch::RandomSearch;

  // Increases the visibility of the internal next parameter function, to bypass general modification, made by the parent class.
  using mant::RandomSearch::nextParametersFunction_;
};

SCENARIO("RandomSearch.nextParametersFunction_", "[RandomSearch][RandomSearch.nextParametersFunction_]") {
  GIVEN("A number of dimension") {
    const arma::uword numberOfDimensions = discreteRandomNumber();
    CAPTURE(numberOfDimensions);

    TestRandomSearch optimisationAlgorithm;

    THEN("Return a randomly and uniformly distributed parameter") {
      arma::Mat<double> paramters(numberOfDimensions, 10000);

      for (arma::uword n = 0; n < paramters.n_cols; ++n) {
        paramters.col(n) = optimisationAlgorithm.nextParametersFunction_(numberOfDimensions, {}, {}, {});
      }

      IS_UNIFORM(arma::vectorise(paramters), 0.0, 1.0);
    }
  }
}

SCENARIO("RandomSearch.optimise", "[RandomSearch][RandomSearch.optimise]") {
  GIVEN("An optimisation problem") {
    WHEN("Called multiple times") {
      const arma::uword numberOfDimensions = SYNCHRONISED(discreteRandomNumber());
      CAPTURE(numberOfDimensions);
      mant::bbob::SphereFunction optimisationProblem(numberOfDimensions);

      mant::RandomSearch optimisationAlgorithm;
      optimisationAlgorithm.setMaximalNumberOfIterations(1);

      THEN("Start with a different parameter") {
        ::mant::isCachingSamples = true;
        for (arma::uword n = 0; n < 100; ++n) {
          optimisationAlgorithm.optimise(optimisationProblem);
        }
        ::mant::isCachingSamples = false;

        CHECK(optimisationProblem.getNumberOfEvaluations() == optimisationProblem.getNumberOfDistinctEvaluations());
      }
    }
  }
}

SCENARIO("RandomSearch.getNextParametersFunctionName", "[RandomSearch][RandomSearch.getNextParametersFunctionName]") {
  mant::RandomSearch optimisationAlgorithm;

  THEN("Return the next parameter function name") {
    CHECK(optimisationAlgorithm.getNextParametersFunctionName() == "Random search");
  }
}
