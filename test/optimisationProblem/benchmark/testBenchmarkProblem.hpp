#include <../ext/catch/catch.hpp>

#include <array>
using std::array;

#include <memory>
using std::unique_ptr;

#include <string>
using std::to_string;

#include <armadillo>
using arma::Mat;
using arma::Col;

#include <optimisationProblem/benchmark/benchmarkProblem.hpp>
using hop::BenchmarkProblem;

#include <optimisationProblem/benchmark/attractiveSectorFunction.hpp>
using hop::AttractiveSectorFunction;

#include <optimisationProblem/benchmark/bentCigarFunction.hpp>
using hop::BentCigarFunction;

#include <optimisationProblem/benchmark/buecheRastriginFunction.hpp>
using hop::BuecheRastriginFunction;

#include <optimisationProblem/benchmark/compositeGriewankRosenbrockFunctionF8F2.hpp>
using hop::CompositeGriewankRosenbrockFunctionF8F2;

#include <optimisationProblem/benchmark/differentPowersFunction.hpp>
using hop::DifferentPowersFunction;

#include <optimisationProblem/benchmark/discusFunction.hpp>
using hop::DiscusFunction;

#include <optimisationProblem/benchmark/ellipsoidalFunction.hpp>
using hop::EllipsoidalFunction;

#include <optimisationProblem/benchmark/ellipsoidalFunctionRotated.hpp>
using hop::EllipsoidalFunctionRotated;

#include <optimisationProblem/benchmark/gallaghersGaussian101mePeaksFunction.hpp>
using hop::GallaghersGaussian101mePeaksFunction;

#include <optimisationProblem/benchmark/gallaghersGaussian21hiPeaksFunction.hpp>
using hop::GallaghersGaussian21hiPeaksFunction;

#include <optimisationProblem/benchmark/katsuuraFunction.hpp>
using hop::KatsuuraFunction;

#include <optimisationProblem/benchmark/linearSlope.hpp>
using hop::LinearSlope;

#include <optimisationProblem/benchmark/lunacekBiRastriginFunction.hpp>
using hop::LunacekBiRastriginFunction;

#include <optimisationProblem/benchmark/rastriginFunction.hpp>
using hop::RastriginFunction;

#include <optimisationProblem/benchmark/rastriginFunctionRotated.hpp>
using hop::RastriginFunctionRotated;

#include <optimisationProblem/benchmark/rosenbrockFunction.hpp>
using hop::RosenbrockFunction;

#include <optimisationProblem/benchmark/rosenbrockFunctionRotated.hpp>
using hop::RosenbrockFunctionRotated;

#include <optimisationProblem/benchmark/schaffersF7Function.hpp>
using hop::SchaffersF7Function;

#include <optimisationProblem/benchmark/schaffersF7FunctionIllConditioned.hpp>
using hop::SchaffersF7FunctionIllConditioned;

#include <optimisationProblem/benchmark/schwefelFunction.hpp>
using hop::SchwefelFunction;

#include <optimisationProblem/benchmark/sharpRidgeFunction.hpp>
using hop::SharpRidgeFunction;

#include <optimisationProblem/benchmark/sphereFunction.hpp>
using hop::SphereFunction;

#include <optimisationProblem/benchmark/stepEllipsoidalFunction.hpp>
using hop::StepEllipsoidalFunction;

#include <optimisationProblem/benchmark/weierstrassFunction.hpp>
using hop::WeierstrassFunction;

#include <iostream>
using std::cout;
using std::endl;

TEST_CASE("Black box optimisation benchmark problem", "[benchmark]") {

  SECTION("test impementation correctness") {
    for(auto numberOfDimensions : {2, 40}) {
      array<unique_ptr<BenchmarkProblem>, 24> bechmarkProblems = {
        unique_ptr<BenchmarkProblem>(new SphereFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new EllipsoidalFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new RastriginFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new BuecheRastriginFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new LinearSlope(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new AttractiveSectorFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new StepEllipsoidalFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new RosenbrockFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new RosenbrockFunctionRotated(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new EllipsoidalFunctionRotated(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new DiscusFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new BentCigarFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new SharpRidgeFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new DifferentPowersFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new RastriginFunctionRotated(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new WeierstrassFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new SchaffersF7Function(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new SchaffersF7FunctionIllConditioned(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new CompositeGriewankRosenbrockFunctionF8F2(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new SchwefelFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new GallaghersGaussian101mePeaksFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new GallaghersGaussian21hiPeaksFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new KatsuuraFunction(numberOfDimensions)),
        unique_ptr<BenchmarkProblem>(new LunacekBiRastriginFunction(numberOfDimensions))
      };

      Mat<double> parameters;
      parameters.load("../test/data/parameters," + to_string(numberOfDimensions) +".mat");

      Col<double> translation;
      translation.load("../test/data/translation," + to_string(numberOfDimensions) +".mat");

      Col<double> one;
      one.load("../test/data/one," + to_string(numberOfDimensions) +".mat");

      Mat<double> rotationR;
      rotationR.load("../test/data/rotationR," + to_string(numberOfDimensions) +".mat");

      Mat<double> rotationQ;
      rotationQ.load("../test/data/rotationQ," + to_string(numberOfDimensions) +".mat");

      Mat<double> deltaC101;
      deltaC101.load("../test/data/deltaC101," + to_string(numberOfDimensions) +".mat");

      Mat<double> localOptimaY101;
      localOptimaY101.load("../test/data/localOptimaY101," + to_string(numberOfDimensions) +".mat");

      Mat<double> deltaC21;
      deltaC21.load("../test/data/deltaC21," + to_string(numberOfDimensions) +".mat");

      Mat<double> localOptimaY21;
      localOptimaY21.load("../test/data/localOptimaY21," + to_string(numberOfDimensions) +".mat");

      for (size_t n = 0; n < bechmarkProblems.size(); n++) {
        Col<double> expected;
        expected.load("../test/data/expectedF" + to_string(n + 1) + "," + to_string(numberOfDimensions) +".mat");

        bechmarkProblems.at(n)->setMaximalNumberOfEvaluations(parameters.n_cols);
        bechmarkProblems.at(n)->setObjectiveValueTranslation(0);
        bechmarkProblems.at(n)->setTranslation(translation);
        bechmarkProblems.at(n)->setOne(one);
        bechmarkProblems.at(n)->setRotationR(rotationR);
        bechmarkProblems.at(n)->setRotationQ(rotationQ);
        bechmarkProblems.at(n)->setDeltaC101(deltaC101);
        bechmarkProblems.at(n)->setLocalOptimaY101(localOptimaY101);
        bechmarkProblems.at(n)->setDeltaC21(deltaC21);
        bechmarkProblems.at(n)->setLocalOptimaY21(localOptimaY21);

        for (size_t j = 0; j < parameters.n_cols; j++) {
          REQUIRE(bechmarkProblems.at(n)->getObjectiveValue(parameters.col(j)) == Approx(expected.at(j)));
        }
      }
    }
  }
}

