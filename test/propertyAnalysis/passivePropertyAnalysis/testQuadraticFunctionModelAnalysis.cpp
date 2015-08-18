// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("QuadraticFunctionModelAnalysis", "") {
  SECTION(".analyse") {
    SECTION("Checking the procedure.") {
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      const arma::Mat<double>& quadraticCoefficients = arma::symmatu(arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfDimensions) * 200 - 100);
      const arma::Col<double>& linearCoefficients = arma::randu<arma::Col<double>>(numberOfDimensions) * 200 - 100;
      
      const arma::uword numberOfParameters = numberOfDimensions + std::uniform_int_distribution<arma::uword>(100, 1000)(mant::Rng::getGenerator());
      const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      
      const double errorTerm = std::uniform_real_distribution<double>(-100, 100)(mant::Rng::getGenerator());
      arma::Col<double> objectiveValues(numberOfParameters);
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        const arma::Col<double>& parameter = parameters.col(n);
        objectiveValues(n) =  arma::dot(parameter.t() * quadraticCoefficients, parameter) + arma::dot(parameter, linearCoefficients) + errorTerm;
      }

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        samples.insert({parameters.col(n), objectiveValues(n)});
      }
      
      mant::QuadraticFunctionModelAnalysis quadraticFunctionModelAnalysis(samples);
      quadraticFunctionModelAnalysis.analyse();
      
      COMPARE(quadraticFunctionModelAnalysis.getQuadraticCoefficients(), quadraticCoefficients);
      COMPARE(quadraticFunctionModelAnalysis.getLinearCoefficients(), linearCoefficients);
      CHECK(quadraticFunctionModelAnalysis.getErrorTerm() == Approx(errorTerm));
      COMPARE(quadraticFunctionModelAnalysis.getResiduals(), arma::zeros<arma::Col<double>>(numberOfParameters));
    } 
  }

  SECTION(".toString") {
    SECTION("Returns the expected class name.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::QuadraticFunctionModelAnalysis(optimisationProblem->getCachedSamples()).toString() == "quadratic_function_model_analysis");
    }
  }
}
