// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("LinearFunctionModel") {
  SECTION(".model") {
  
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      mant::LinearFunctionModelAnalysis linearFunctionModelAnalysis(optimisationProblem->getCachedSamples());
      linearFunctionModelAnalysis.analyse();
      CHECK(mant::LinearFunctionModel(linearFunctionModelAnalysis.getCoefficients(), linearFunctionModelAnalysis.getErrorTerm()).toString() == "linear_function_model");
    }
  }
}
