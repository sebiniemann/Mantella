// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ standard library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("bbob::LunacekBiRastriginFunction") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::LunacekBiRastriginFunction lunacekBiRastriginFunction(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Col<double> one;
    REQUIRE(one.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_one_" + std::to_string(numberOfDimensions) +"x1.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_2.input"));

    arma::Mat<double> rotationQ;
    REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_1.input"));

    arma::Col<double> scaling;
    REQUIRE(scaling.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_scaling_" + std::to_string(numberOfDimensions) +"x1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_lunacekBiRastriginFunction_dim" + std::to_string(numberOfDimensions) +".expected"));

    lunacekBiRastriginFunction.setObjectiveValueTranslation(0);
    lunacekBiRastriginFunction.setParameterScaling(scaling);
    lunacekBiRastriginFunction.setRotationR(rotationR);
    lunacekBiRastriginFunction.setRotationQ(rotationQ);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(lunacekBiRastriginFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      CHECK(mant::bbob::LunacekBiRastriginFunction(5).toString() == "bbob_lunacek_bi_rastrigin_function");
    }
  }
}
