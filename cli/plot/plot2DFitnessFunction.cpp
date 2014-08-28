#include <plot/plot2DFitnessFunction.hpp>

#include <cstdlib>
#include <fstream>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <config.hpp>

Plot2DFitnessFunction::Plot2DFitnessFunction(const std::shared_ptr<hop::OptimisationProblem> optimisationProblem) {
  _optimisationProblem = optimisationProblem;
}

void Plot2DFitnessFunction::runImpelementation() {
  arma::Col<double> xAxis = arma::linspace(_optimisationProblem->getLowerBounds().at(0), _optimisationProblem->getUpperBounds().at(0), 250);
  arma::Col<double> yAxis = arma::linspace(_optimisationProblem->getLowerBounds().at(1), _optimisationProblem->getUpperBounds().at(1), 250);

  std::ofstream file(Config::getGnuplotScriptPath() + "fitnessFunction.dat");
  file << "# X Y Z" << "\n";

  for (std::size_t yIndex = 0; yIndex < yAxis.n_elem; yIndex++) {
    double y = yAxis.at(yIndex);
    for (std::size_t xIndex = 0; xIndex < xAxis.n_elem; xIndex++) {
      double x = xAxis.at(xIndex);
      file << x << " " << y << " " << _optimisationProblem->getObjectiveValue({x, y}) << "\n";
    }
    file << "\n";
  }
  file.close();

  char workingDirectory[1000];
  getcwd(workingDirectory, sizeof(workingDirectory));
  chdir(Config::getGnuplotScriptPath().c_str());
  system((Config::getGnuplotBinPath() + "gnuplot plotFitnessFunction.txt").c_str());
  chdir(workingDirectory);
}

CEREAL_REGISTER_TYPE(Plot2DFitnessFunction)
