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

TEST_CASE("bbob::SchaffersF7Function", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob::SchaffersF7Function schaffersF7Function(numberOfDimensions);

    arma::Mat<double> parameters;
    REQUIRE(parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_" + std::to_string(numberOfDimensions) + "x10.input"));

    arma::Col<double> translation;
    REQUIRE(translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_" + std::to_string(numberOfDimensions) + "x1.input"));

    arma::Mat<double> rotationR;
    REQUIRE(rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_2.input"));

    arma::Mat<double> rotationQ;
    REQUIRE(rotationQ.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_" + std::to_string(numberOfDimensions) + "x" + std::to_string(numberOfDimensions) + "_1.input"));

    arma::Col<double> expected;
    REQUIRE(expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_schaffersF7Function_dim" + std::to_string(numberOfDimensions) +".expected"));

    schaffersF7Function.setObjectiveValueTranslation(0);
    schaffersF7Function.setParameterTranslation(translation);
    schaffersF7Function.setParameterRotation(rotationR);
    schaffersF7Function.setRotationQ(rotationQ);

    for (arma::uword n = 0; n < parameters.n_cols; ++n) {
      CHECK(schaffersF7Function.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::SchaffersF7Function(5).toString() == "bbob_schaffers_f7_function");
  }
}
