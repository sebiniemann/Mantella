// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

extern std::string testDirectory;

TEST_CASE("BBOB2015-EllipsoidalFunctionRotated", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2015::EllipsoidalFunctionRotated ellipsoidalFunctionRotated(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> translation;
    translation.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/expectedEllipsoidalFunctionRotated,dim" + std::to_string(numberOfDimensions) +".mat");

    ellipsoidalFunctionRotated.setObjectiveValueTranslation(0);
    ellipsoidalFunctionRotated.setTranslation(translation);
    ellipsoidalFunctionRotated.setRotationR(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(ellipsoidalFunctionRotated.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2009::AttractiveSectorFunction(5).to_string() == "EllipsoidalFunctionRotated");
  }
}
