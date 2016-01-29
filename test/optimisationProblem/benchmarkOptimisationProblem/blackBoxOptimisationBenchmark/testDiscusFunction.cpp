// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

class TestDiscusFunction : public mant::bbob::DiscusFunction {
 public:
  using mant::bbob::DiscusFunction::DiscusFunction;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::bbob::DiscusFunction::objectiveFunction_;
};

SCENARIO("bbob::DiscusFunction.objectiveFunction_", "[bbob::DiscusFunction][bbob::DiscusFunction.objectiveFunction_]") {
  GIVEN("Some parameters") {
    // All expected objective values where generated for a 40-dimensional problem instance.
    const arma::uword numberOfDimensions = 40;
    CAPTURE(numberOfDimensions);
    TestDiscusFunction optimisationProblem(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(::rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

    THEN("Return their objective value") {
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(::rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/discusFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(optimisationProblem.objectiveFunction_(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }
}

SCENARIO("bbob::DiscusFunction.getNormalisedObjectiveValue", "[bbob::DiscusFunction][bbob::DiscusFunction.getNormalisedObjectiveValue]") {
  GIVEN("A parameter") {
    const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
    CAPTURE(numberOfDimensions);

    const arma::Col<double>& parameter = arma::normalise(continuousRandomNumbers(numberOfDimensions));
    CAPTURE(parameter);

    WHEN("Instantiated multiple times") {
      THEN("Return a different objective value (due to randomised initialisation)") {
        arma::uword numberOfCollisions = 0;

        double previousObjectiveValue = arma::datum::inf;
        for (arma::uword n = 0; n < 100; ++n) {
          mant::bbob::DiscusFunction optimisationProblem(numberOfDimensions);

          const double objectiveValue = optimisationProblem.getNormalisedObjectiveValue(parameter);
          if (std::abs(previousObjectiveValue - objectiveValue) <= 0) {
            ++numberOfCollisions;
          }
          previousObjectiveValue = objectiveValue;
        }

        // Since some random values are truncated, collisions are unavoidably. However, there shouldn't be more then 10 collisions out of 100.
        CHECK(numberOfCollisions <= 10);
      }
    }

#if defined(SUPPORT_MPI)
    WHEN("MPI is supported") {
      mant::bbob::DiscusFunction optimisationProblem(numberOfDimensions);

      THEN("Return the same objective value for each node") {
        double objectiveValue = optimisationProblem.getNormalisedObjectiveValue(SYNCHRONISED(parameter));

        arma::Col<double> objectiveValues(static_cast<arma::uword>(numberOfNodes));
        MPI_Allgather(&objectiveValue, 1, MPI_DOUBLE, objectiveValues.memptr(), 1, MPI_DOUBLE, MPI_COMM_WORLD);
        CAPTURE(objectiveValues);

        CHECK(arma::all(objectiveValues == objectiveValue));
      }
    }
#endif
  }
}

SCENARIO("bbob::DiscusFunction.getObjectiveFunctionName", "[bbob::DiscusFunction][bbob::DiscusFunction.getObjectiveFunctionName]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(1 + discreteRandomNumber());
  CAPTURE(numberOfDimensions);
  mant::bbob::DiscusFunction optimisationProblem(numberOfDimensions);

  THEN("Return the objective function name") {
    CHECK(optimisationProblem.getObjectiveFunctionName() == "BBOB Discus Function (f11)");
  }
}
