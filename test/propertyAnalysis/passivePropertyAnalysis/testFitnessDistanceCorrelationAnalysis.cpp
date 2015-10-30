// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("FitnessDistanceCorrelationAnalysis") {
  SECTION(".analyse") {
    SECTION("Checking the procedure.") {
      arma::Mat<double>::fixed<11, 2> firstOfAnscombesQuartet({10.0, 8.0, 13.0, 9.0, 11.0, 14.0, 6.0, 4.0, 12.0, 7.0, 5.0, 8.040, 6.950, 7.580, 8.810, 8.330, 9.960, 7.240, 4.260, 10.840, 4.820, 5.680});

      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());

      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      samples.insert({arma::zeros<arma::Col<double>>(numberOfDimensions), 0.0});
      for (arma::uword n = 0; n < firstOfAnscombesQuartet.n_rows; ++n) {
        samples.insert({arma::normalise(arma::randn<arma::Col<double>>(numberOfDimensions)) * firstOfAnscombesQuartet(n, 0), firstOfAnscombesQuartet(n, 1)});
      }

      mant::FitnessDistanceCorrelationAnalysis fitnessDistanceCorrelationAnalysis(samples);
      fitnessDistanceCorrelationAnalysis.analyse();

      CHECK(fitnessDistanceCorrelationAnalysis.getCorrelationCoefficient() == Approx(0.8164205163));
    }
  }

  SECTION(".toString") {
    SECTION("Returns a (filesystem friendly) name for the class.") {
      mant::cacheSamples = true;
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.optimise();
      CHECK(mant::FitnessDistanceCorrelationAnalysis(optimisationProblem->getCachedSamples()).toString() ==
            "fitness_distance_correlation_analysis");
    }
  }
}
