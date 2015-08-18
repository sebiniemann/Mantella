// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>
#include <random>
#include <unordered_map>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("LinearFunctionModelAnalysis", "") {
  SECTION(".analyse") {
    SECTION("Checking the procedure.") {
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      const arma::Col<double>& coefficients = arma::randu<arma::Col<double>>(numberOfDimensions) * 200 - 100;
      
      const arma::uword numberOfParameters = numberOfDimensions + std::uniform_int_distribution<arma::uword>(1, 100)(mant::Rng::getGenerator());
      const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      
      const double errorTerm = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      arma::Col<double> objectiveValues(numberOfParameters);
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        objectiveValues(n) = arma::dot(parameters.col(n), coefficients) + errorTerm;
      }

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        samples.insert({parameters.col(n), objectiveValues(n)});
      }
      
      mant::LinearFunctionModelAnalysis linearFunctionModelAnalysis(samples);
      linearFunctionModelAnalysis.analyse();
      
      COMPARE(linearFunctionModelAnalysis.getCoefficients(), coefficients);
      CHECK(linearFunctionModelAnalysis.getErrorTerm() == Approx(errorTerm));
      COMPARE(linearFunctionModelAnalysis.getResiduals(), arma::zeros<arma::Col<double>>(numberOfParameters));
    } 
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::LinearFunctionModelAnalysis(optimisationProblem->getCachedSamples()).toString() == "linear_function_model_analysis");
    }
  }
}