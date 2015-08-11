// Catch
#include <catch.hpp>
#include <catchExtension.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("FitnessDistanceCorrelationAnalysis", "") {
  SECTION(".analyse") {
    SECTION("Checking the procedure.") {
      arma::Mat<double>::fixed<44, 2> anscombesQuartet({
        10.0, 8.0, 13.0, 9.0, 11.0, 14.0, 6.0, 4.0, 12.0, 7.0, 5.0, 10.0, 8.0, 13.0, 9.0, 11.0, 14.0, 6.0, 4.0, 12.0, 7.0, 5.0, 10.0, 8.0, 13.0, 9.0, 11.0, 14.0, 6.0, 4.0, 12.0, 7.0, 5.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 19.0, 8.0, 8.0, 8.0,
        8.040, 6.950, 7.580, 8.810, 8.330, 9.960, 7.240, 4.260, 10.840, 4.820, 5.680, 9.140, 8.140, 8.740, 8.770, 9.260, 8.10, 6.130, 3.10, 9.130, 7.260, 4.740, 7.460, 6.770, 12.740, 7.110, 7.810, 8.840, 6.080, 5.390, 8.150, 6.420, 5.730, 6.580, 5.760, 7.710, 8.840, 8.470, 7.040, 5.250, 12.50, 5.560, 7.910, 6.890
      });
      
      const arma::uword numberOfDimensions = std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator());
      
      std::unordered_map<arma::Col<double>, double, mant::Hash, mant::IsEqual> samples;
      samples.insert({arma::zeros<arma::Col<double>>(numberOfDimensions), 0.0});
      for (arma::uword n = 0; n < anscombesQuartet.n_rows; ++n) {
        samples.insert({arma::normalise(arma::randn<arma::Col<double>>(numberOfDimensions)) * anscombesQuartet(n, 0), anscombesQuartet(n, 1)});
      }
      
      mant::FitnessDistanceCorrelationAnalysis fitnessDistanceCorrelationAnalysis(samples);
      fitnessDistanceCorrelationAnalysis.analyse();
      
      CHECK(fitnessDistanceCorrelationAnalysis.getCorrelationCoefficient() == Approx(0.816366242761470));
    }
  }

  SECTION("Returns the specified class name.") {
    mant::cacheSamples = true;
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(std::uniform_int_distribution<arma::uword>(1, 10)(mant::Rng::getGenerator())));
    mant::RandomSearch randomSearch(optimisationProblem);
    randomSearch.optimise();
    CHECK(mant::FitnessDistanceCorrelationAnalysis(optimisationProblem->getCachedSamples()).toString() == "fitness_distance_correlation_analysis");
  }
}