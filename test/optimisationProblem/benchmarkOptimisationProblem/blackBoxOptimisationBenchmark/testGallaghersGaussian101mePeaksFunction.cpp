// Catch
#include <catch.hpp>
#include "catchExtension.hpp"

// Mantella
#include <mantella>

extern std::string testDirectory;

class TestGallaghersGaussian101mePeaksFunction : public mant::bbob::GallaghersGaussian101mePeaksFunction {
 public:
  using mant::bbob::GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::GallaghersGaussian101mePeaksFunction::localParameterConditionings_;
  using mant::bbob::GallaghersGaussian101mePeaksFunction::localParameterTranslations_;
  using mant::bbob::GallaghersGaussian101mePeaksFunction::rotationQ_;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::bbob::GallaghersGaussian101mePeaksFunction::objectiveFunction_;
};

SCENARIO("bbob::GallaghersGaussian101mePeaksFunction.objectiveFunction_", "[bbob::GallaghersGaussian101mePeaksFunction][bbob::GallaghersGaussian101mePeaksFunction.objectiveFunction_]") {
  GIVEN("Some parameters") {
    // All expected objective values where generated for a 40-dimensional problem instance.
    const arma::uword numberOfDimensions = 40;
    CAPTURE(numberOfDimensions);
    TestGallaghersGaussian101mePeaksFunction optimisationProblem(numberOfDimensions);

    REQUIRE(optimisationProblem.rotationQ_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_rotationMatrix_40x40_1.input"));
    REQUIRE(optimisationProblem.localParameterConditionings_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_localParameterConditionings_40x101.input"));
    REQUIRE(optimisationProblem.localParameterTranslations_.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_localParameterTranslations_40x101.input"));

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x100.input"));

    THEN("Return their objective value") {
      arma::Col<double> expectedObjectiveValues;
      REQUIRE(expectedObjectiveValues.load(rootTestDataDirectory + "/optimisationProblem/benchmarkOptimisationProblem/blackBoxOptimisationBenchmark/gallaghersGaussian101mePeaksFunction_dim40_1x100.expected"));

      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        CHECK(optimisationProblem.objectiveFunction_(parameters.col(n)) == Approx(expectedObjectiveValues(n)));
      }
    }
  }
}

SCENARIO("bbob::GallaghersGaussian101mePeaksFunction.getNormalisedObjectiveValue", "[bbob::GallaghersGaussian101mePeaksFunction][bbob::GallaghersGaussian101mePeaksFunction.getNormalisedObjectiveValue]") {
  GIVEN("A parameter") {
    const arma::uword numberOfDimensions = SYNCHRONISED(1 + getDiscreteRandomNumber());
    CAPTURE(numberOfDimensions);

    const arma::Col<double>& parameter = arma::normalise(getContinuousRandomNumbers(numberOfDimensions));
    CAPTURE(parameter);

    WHEN("Instantiated multiple times") {
      THEN("Return a different objective value (due to randomised initialisation)") {
        arma::uword numberOfCollisions = 0;

        double previousObjectiveValue = arma::datum::inf;
        for (arma::uword n = 0; n < 100; ++n) {
          TestGallaghersGaussian101mePeaksFunction optimisationProblem(numberOfDimensions);

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
      mant::bbob::GallaghersGaussian101mePeaksFunction optimisationProblem(numberOfDimensions);

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

SCENARIO("bbob::GallaghersGaussian101mePeaksFunction.getObjectiveFunctionName", "[bbob::GallaghersGaussian101mePeaksFunction][bbob::GallaghersGaussian101mePeaksFunction.getObjectiveFunctionName]") {
  const arma::uword numberOfDimensions = SYNCHRONISED(1 + getDiscreteRandomNumber());
  CAPTURE(numberOfDimensions);
  mant::bbob::GallaghersGaussian101mePeaksFunction optimisationProblem(numberOfDimensions);

  THEN("Return the objective function name") {
    CHECK(optimisationProblem.getObjectiveFunctionName() == "BBOB Gallagher's Gaussian 101-me Peaks Function (f21)");
  }
}
