// Catch
#include <catch.hpp>

// C++ standard library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE(
    "RandomSearch") {
  SECTION(
      ".optimise") {
    SECTION(
        "Checks if all parameters are uniformly distributed.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
      optimisationProblem->setLowerBounds(arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 200 - 100);
      optimisationProblem->setUpperBounds(optimisationProblem->getLowerBounds() + arma::randu<arma::Col<double>>(optimisationProblem->numberOfDimensions_) * 100 + 0.01);

      mant::recordSamples = true;
      mant::RandomSearch randomSearch(optimisationProblem);
      randomSearch.setMaximalNumberOfIterations(100000);
      randomSearch.optimise();

      std::vector<std::pair<arma::Col<double>, double>> actualSamples = randomSearch.getSamplingProgress();

      std::vector<arma::Col<double>> intervals;
      for (arma::uword n = 0; n < optimisationProblem->numberOfDimensions_; ++n) {
        intervals.push_back(arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds()(n), optimisationProblem->getUpperBounds()(n), 11));
      }

      arma::uword minimalNumberOfSamplesPerSubset = randomSearch.getMaximalNumberOfIterations();
      arma::uword maximalNumberOfSamplesPerSubset = 0;
      for (arma::uword n = 1; n < intervals.at(0).n_elem; ++n) {
        for (arma::uword k = 1; k < intervals.at(1).n_elem; ++k) {
          arma::uword numberOfSamples = 0;
          for (const auto& sample : actualSamples) {
            const arma::Col<double>& parameter = sample.first;

            if (parameter(0) >= intervals.at(0)(n - 1) && parameter(0) < intervals.at(0)(n) && parameter(1) >= intervals.at(1)(k - 1) && parameter(1) < intervals.at(1)(k)) {
              ++numberOfSamples;
            }

            minimalNumberOfSamplesPerSubset = std::min(minimalNumberOfSamplesPerSubset, numberOfSamples);
            maximalNumberOfSamplesPerSubset = std::max(maximalNumberOfSamplesPerSubset, numberOfSamples);
          }
        }
      }

      CHECK(0.25 > static_cast<double>(maximalNumberOfSamplesPerSubset - minimalNumberOfSamplesPerSubset) / static_cast<double>(actualSamples.size()));
    }
  }

  SECTION(
      ".toString") {
    SECTION(
        "Returns the expected class name.") {
      std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob::SphereFunction(2));
      CHECK(mant::RandomSearch(optimisationProblem).toString() ==
            "random_search");
    }
  }
}
