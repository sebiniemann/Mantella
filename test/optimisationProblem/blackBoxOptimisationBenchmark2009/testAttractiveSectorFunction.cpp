// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Boost
#include <boost/filesystem.hpp>

// Mantella
#include <mantella>

#include <iostream>

extern boost::filesystem::path testDirectory;

TEST_CASE("BBOB2009-AttractiveSectorFunction", "") {
  std::cout << "ZZZ" << std::endl;
  for (const auto& numberOfDimensions : {2, 40}) {
    std::cout << "QQQ" << numberOfDimensions << std::endl;
    mant::bbob2009::AttractiveSectorFunction attractiveSectorFunction(numberOfDimensions);

    std::cout << "AAA" << numberOfDimensions << std::endl;

    arma::Mat<double> parameters;
    parameters.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/parameters,dim" + std::to_string(numberOfDimensions) +".mat");

    std::cout << "BBB" << numberOfDimensions << std::endl;

    arma::Col<double> translation;
    translation.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/translation,dim" + std::to_string(numberOfDimensions) +".mat");

    std::cout << "CCC" << numberOfDimensions << std::endl;

    arma::Mat<double> rotationR;
    rotationR.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationR,dim" + std::to_string(numberOfDimensions) +".mat");

    std::cout << "DDD" << numberOfDimensions << std::endl;

    arma::Mat<double> rotationQ;
    rotationQ.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/rotationQ,dim" + std::to_string(numberOfDimensions) +".mat");

    std::cout << "EEE" << numberOfDimensions << std::endl;

    arma::Col<double> expected;
    expected.load(testDirectory.string() + "/data/optimisationProblem/blackBoxOptimisationBenchmark2013/expectedAttractiveSectorFunction,dim" + std::to_string(numberOfDimensions) +".mat");

    std::cout << "FFF" << numberOfDimensions << std::endl;

    attractiveSectorFunction.setObjectiveValueTranslation(0);

    std::cout << "GGG" << numberOfDimensions << std::endl;
    attractiveSectorFunction.setTranslation(translation);

    std::cout << "HHH" << numberOfDimensions << std::endl;
    attractiveSectorFunction.setRotationR(rotationR);

    std::cout << "III" << numberOfDimensions << std::endl;
    attractiveSectorFunction.setRotationQ(rotationQ);

    for (std::size_t n = 0; n < parameters.n_cols; ++n) {

      std::cout << "JJJ" << numberOfDimensions << "###" << n << std::endl;
      CHECK(attractiveSectorFunction.getObjectiveValue(parameters.col(n)) == Approx(expected.at(n)));
      std::cout << "TTT" << numberOfDimensions << "###" << n << std::endl;
    }
    std::cout << "SSS" << numberOfDimensions << std::endl;
  }
  std::cout << "RRR" << std::endl;
}

