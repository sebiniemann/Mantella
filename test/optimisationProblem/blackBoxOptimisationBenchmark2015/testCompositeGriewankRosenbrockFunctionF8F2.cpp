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

TEST_CASE("bbob2015::CompositeGriewankRosenbrockFunctionF8F2", "") {
  for (const auto& numberOfDimensions : {2, 40}) {
    mant::bbob2015::CompositeGriewankRosenbrockFunctionF8F2 compositeGriewankRosenbrockFunctionF8F2(numberOfDimensions);

    arma::Mat<double> parameters;
    parameters.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    arma::Col<double> expected;
    expected.load(testDirectory + "/data/optimisationProblem/blackBoxOptimisationBenchmark2015/expectedCompositeGriewankRosenbrockFunctionF8F2,dim" + std::to_string(numberOfDimensions) +".mat");

    compositeGriewankRosenbrockFunctionF8F2.setObjectiveValueTranslation(0);
    compositeGriewankRosenbrockFunctionF8F2.setRotationR(rotationR);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(compositeGriewankRosenbrockFunctionF8F2.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob2009::AttractiveSectorFunction(5).to_string() == "CompositeGriewankRosenbrockFunctionF8F2");
  }
}
