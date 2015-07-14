// Catch
#include <catch.hpp>

// C++ Standard Library
#include <cstdlib>

// Armadillo
#include <armadillo>

// Mantella
#include <mantella>

TEST_CASE("RandomSerach.optimise()", "") {
  std::shared_ptr<mant::OptimisationProblem<>> optimisationProblem(new mant::bbob::SphereFunction<>(2));
  mant::RandomSearch<> randomSearch(optimisationProblem);
  randomSearch.setMaximalNumberOfIterations(1000000);
  randomSearch.optimise();

  std::vector<std::pair<arma::Col<double>, double>> history = randomSearch.getParameterHistory();

  SECTION("Checks if the parameters are uniformly distributed"){
    for (double dimension : arma::linspace(0, optimisationProblem->numberOfDimensions_ - 1, optimisationProblem->numberOfDimensions_)) {
      std::vector<double> marks;
      for (const auto& mark : history) {
        marks.push_back(mark.first.at(dimension));
      }

      arma::Col<unsigned int> histogram = arma::hist(arma::Col<double>(marks), arma::linspace<arma::Col<double>>(optimisationProblem->getLowerBounds().at(dimension), optimisationProblem->getUpperBounds().at(dimension), 100));
      CHECK(0.05 > static_cast<double>(histogram.max() - histogram.min()) / marks.size());
    }
  }
}
