// Catch
#include <catch.hpp>

// C++ standard library
#include <random>
#include <memory>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("RadialBasisFunctionModel") {
  SECTION(".model") {
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::RadialBasisFunctionModel(optimisationProblem->getCachedSamples(), std::shared_ptr<mant::RadialBasisFunction>(new mant::GaussianRadialBasisFunction(optimisationProblem->numberOfDimensions_, 1))).toString() ==
            "radial_basis_function_model");
    }
  }
}
