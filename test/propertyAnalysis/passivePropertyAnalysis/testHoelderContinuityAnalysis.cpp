// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("HoelderContinuityAnalysis") {
  SECTION(".analyse") {
    SECTION("Checking the procedure.") {
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      const arma::uword numberOfParameters = numberOfDimensions + std::uniform_int_distribution<arma::uword>(1, 100)(mant::Rng::getGenerator());

      const arma::Mat<double>& parameters = arma::randu<arma::Mat<double>>(numberOfDimensions, numberOfParameters) * 200 - 100;
      const arma::Col<double>& objectiveValues = arma::randu<arma::Col<double>>(numberOfParameters) * 200 - 100;

      double hoelderExponent = 1;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        for (arma::uword k = n + 1; k < parameters.n_cols; ++k) {
          hoelderExponent = std::min(hoelderExponent, std::log(std::abs((objectiveValues(n) - objectiveValues(k)))) / std::log(arma::norm(parameters.col(n) - parameters.col(k))));
        }
      }

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        samples.insert({parameters.col(n), objectiveValues(n)});
      }

      mant::HoelderContinuityAnalysis hoelderContinuityAnalysis(samples);
      hoelderContinuityAnalysis.analyse();

      CHECK(hoelderContinuityAnalysis.getHoelderExponent() == Approx(hoelderExponent));
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::HoelderContinuityAnalysis(optimisationProblem->getCachedSamples()).toString() ==
            "hoelder_continuity_analysis");
    }
  }
}
