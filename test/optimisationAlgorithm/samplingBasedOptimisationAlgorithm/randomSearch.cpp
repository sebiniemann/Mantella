// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("RandomSerach.optimise()", "") {
  std::shared_ptr<mant::OptimisationProblem<>> randomProblem(new mant::bbob::SphereFunction<>(2));
  mant::RandomSearch<> testRandomSearch(randomProblem);
  testRandomSearch.setMaximalNumberOfIterations(1000000);
  testRandomSearch.optimise();

  std::vector<std::pair<arma::Col<double>,double>> history = testRandomSearch.getParameterHistory();

  SECTION("Checks if the parameters are uniformly distributed"){
    for (double dimension : arma::linspace(0, randomProblem->numberOfDimensions_ - 1, randomProblem->numberOfDimensions_)) {
      std::vector<double> marks;
      for (auto mark : history) {
        marks.push_back(mark.first.at(dimension));
      }

      arma::Col<unsigned int> histogram = arma::hist(arma::Col<double>(marks), arma::linspace<arma::Col<double>>(-5.0, 5.0, 100));
      CHECK(0.05 > static_cast<double>(histogram.max() - histogram.min()) / marks.size());
    }
  }
}
