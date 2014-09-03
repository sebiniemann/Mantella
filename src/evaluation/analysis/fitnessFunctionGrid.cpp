#include <script/fitnessFunctionGrid.hpp>

#include <cstdlib>
#include <fstream>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <config.hpp>

FitnessFunctionGrid::FitnessFunctionGrid(const std::shared_ptr<hop::OptimisationProblem> optimisationProblem) {
  _optimisationProblem = optimisationProblem;
}

void FitnessFunctionGrid::runImpelementation() {
  arma::Col<double> xAxis = arma::linspace(_optimisationProblem->getLowerBounds().at(0), _optimisationProblem->getUpperBounds().at(0), 250);
  arma::Col<double> yAxis = arma::linspace(_optimisationProblem->getLowerBounds().at(1), _optimisationProblem->getUpperBounds().at(1), 250);

  std::ofstream file(Config::getGnuplotScriptPath() + "fitnessFunction.dat");
  file << "# X Y Z" << "\n";

  for (std::size_t yIndex = 0; yIndex < yAxis.n_elem; yIndex++) {
    double y = yAxis.at(yIndex);
    for (std::size_t xIndex = 0; xIndex < xAxis.n_elem; xIndex++) {
      double x = xAxis.at(xIndex);
      file << x << " " << y << " " << _optimisationProblem->getObjectiveValue({x, y}) << "\n";
      setProgress((yIndex * xAxis.n_elem + xIndex) / (yAxis.n_elem * xAxis.n_elem + 9));
    }
    file << "\n";
  }
  file.close();
}

CEREAL_REGISTER_TYPE(FitnessFunctionGrid)
