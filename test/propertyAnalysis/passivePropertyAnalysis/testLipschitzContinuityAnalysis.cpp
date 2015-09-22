// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "LipschitzContinuityAnalysis") {
  SECTION(
      ".analyse") {
    SECTION(
        "Checking the procedure.") {
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      const arma::uword numberOfParameters = numberOfDimensions + std::uniform_int_distribution<arma::uword>(1, 100)(mant::Rng::getGenerator());

      const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;

      double lipschitzConstant = 0;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
          lipschitzConstant = std::max(lipschitzConstant, std::abs((objectiveValues(n) - objectiveValues(k)) / arma::norm(parameters.col(n) - parameters.col(k))));
        }
      }

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        samples.insert({parameters.col(n), objectiveValues(n)});
      }

      mant::LipschitzContinuityAnalysis lipschitzContinuityAnalysis(samples);
      lipschitzContinuityAnalysis.analyse();

      CHECK(lipschitzContinuityAnalysis.getLipschitzConstant() == Approx(lipschitzConstant));
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::LipschitzContinuityAnalysis(optimisationProblem->getCachedSamples()).toString() ==
            "lipschitz_continuity_analysis");
    }
  }
}
