// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("QuadraticFunctionModel") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      mant::QuadraticFunctionModelAnalysis quadraticFunctionModelAnalysis(optimisationProblem->getCachedSamples());
      quadraticFunctionModelAnalysis.analyse();
      CHECK(mant::QuadraticFunctionModel(quadraticFunctionModelAnalysis.getQuadraticCoefficients(), quadraticFunctionModelAnalysis.getLinearCoefficients(), quadraticFunctionModelAnalysis.getErrorTerm()).toString() == "quadratic_function_model");
    }
  }
}
