#include <hop_bits/evaluation/fitnessFunctionGrid.hpp>

#include <cstdlib>
#include <fstream>

#include <hop_bits/config.hpp>

namespace hop {
  FitnessFunctionGrid::FitnessFunctionGrid(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : optimisationProblem_(optimisationProblem) {
    setLowerBounds(optimisationProblem_->getLowerBounds());
    setUpperBounds(optimisationProblem_->getUpperBounds());
  }

  void FitnessFunctionGrid::setLowerBounds(const arma::Col<double>& lowerBounds) {
    lowerBounds_ = lowerBounds;
  }

  void FitnessFunctionGrid::setUpperBounds(const arma::Col<double>& upperBounds) {
    upperBounds_ = upperBounds;
  }

  void FitnessFunctionGrid::runImpelementation() {
    arma::Col<double> xAxis = arma::linspace(lowerBounds_.at(0), upperBounds_.at(0), 50);
    arma::Col<double> yAxis = arma::linspace(lowerBounds_.at(1), upperBounds_.at(1), 50);

    arma::Mat<double> fitnessFunction(xAxis.n_elem * yAxis.n_elem, 3);

    std::size_t n = 0;
    for (std::size_t xIndex = 0; xIndex < xAxis.n_elem; ++xIndex) {
      double x = xAxis.at(xIndex);
      for (std::size_t yIndex = 0; yIndex < yAxis.n_elem; ++yIndex) {
        double y = yAxis.at(yIndex);
        fitnessFunction.row(n++) = arma::Row<double>({x, y, optimisationProblem_->getObjectiveValue({x, y})});
        setProgress(n / (yAxis.n_elem * xAxis.n_elem));
      }
    }

    save(fitnessFunction, "fitnessFunction");
  }
}
