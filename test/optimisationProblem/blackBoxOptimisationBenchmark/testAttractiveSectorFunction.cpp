// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("bbob::AttractiveSectorFunction", "") {
  arma::Mat<double> parameters;
  arma::Col<double> translation;
  arma::Mat<double> rotationR;
  arma::Mat<double> rotationQ;
  arma::Col<double> expected;

  SECTION("Dimmension 2"){
    mant::bbob::AttractiveSectorFunction<> attractiveSectorFunction(2);

    parameters = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_2x10.input"
    };
    parameters.reshape(2,10);

    translation = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_2x1.input"
    };
    translation.reshape(2,1);

    rotationR = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_2x2_2.input"
    };
    rotationR.reshape(2,2);

    rotationQ = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_2x2_1.input"
    };
    rotationQ.reshape(2,2);

    expected = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_attractiveSectorFunction_dim2.expected"
    };
    expected.reshape(10,1);

    attractiveSectorFunction.setObjectiveValueTranslation(0);
    attractiveSectorFunction.setParameterTranslation(translation);
    attractiveSectorFunction.setParameterRotation(rotationR);
    attractiveSectorFunction.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(attractiveSectorFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }

  SECTION("Dimmension 40"){
    mant::bbob::AttractiveSectorFunction<> attractiveSectorFunction(40);

    parameters = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_parameters_40x10.input"
    };
    parameters.reshape(40,10);

    translation = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_translation_40x1.input"
    };
    translation.reshape(40,1);

    rotationR = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_40x40_2.input"
    };
    rotationR.reshape(40,40);

    rotationQ = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/_randomRotationMatrix_40x40_1.input"
    };
    rotationQ.reshape(40,40);

    expected = {
  #include "../../data/optimisationProblem/blackBoxOptimisationBenchmark/bbob_attractiveSectorFunction_dim40.expected"
    };
    expected.reshape(10,1);

    attractiveSectorFunction.setObjectiveValueTranslation(0);
    attractiveSectorFunction.setParameterTranslation(translation);
    attractiveSectorFunction.setParameterRotation(rotationR);
    attractiveSectorFunction.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {
      CHECK(attractiveSectorFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
    }
  }


  SECTION("Returns the specified class name.") {
    CHECK(mant::bbob::AttractiveSectorFunction<>(5).toString() == "bbob_attractive_sector_function");
  }
}

