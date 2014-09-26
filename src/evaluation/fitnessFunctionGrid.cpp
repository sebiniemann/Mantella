#include <hop_bits/evaluation/fitnessFunctionGrid.hpp>

#include <cstdlib>
#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <hop_bits/helper/to_string.hpp>
#include <hop_bits/helper/cereal.hpp>

namespace hop {
  FitnessFunctionGrid::FitnessFunctionGrid(const std::shared_ptr<OptimisationProblem> optimisationProblem)
    : optimisationProblem_(optimisationProblem) {
    setLowerBounds(optimisationProblem_->getLowerBounds());
    setUpperBounds(optimisationProblem_->getUpperBounds());
    setGridSize(arma::Col<arma::uword>({100, 100}));
  }

  void FitnessFunctionGrid::setLowerBounds(const arma::Col<double>& lowerBounds) {
    lowerBounds_ = lowerBounds;
  }

  void FitnessFunctionGrid::setUpperBounds(const arma::Col<double>& upperBounds) {
    upperBounds_ = upperBounds;
  }

  void FitnessFunctionGrid::setGridSize(const arma::Col<arma::uword>& gridSize) {
    gridSize_ = gridSize;
  }

  void FitnessFunctionGrid::runImpelementation() {
    if(optimisationProblem_->getNumberOfDimensions() != 2) {
      // TODO Add exception
    }

    arma::Col<double> xAxis = arma::linspace(lowerBounds_.at(0), upperBounds_.at(0), gridSize_.at(0));
    arma::Col<double> yAxis = arma::linspace(lowerBounds_.at(1), upperBounds_.at(1), gridSize_.at(1));

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

    save(fitnessFunction,
         "fitnessFunction_" +
         to_string(optimisationProblem_) +
         "with_" +
         std::to_string(gridSize_.at(0)) +
         "_x_from_" +
         std::to_string(lowerBounds_.at(0)) +
         "_to_" +
         std::to_string(upperBounds_.at(0)) +
         "_and_" +
         std::to_string(gridSize_.at(1)) +
         "_y_from_" +
         std::to_string(lowerBounds_.at(1)) +
         "_to_" +
         std::to_string(upperBounds_.at(1)));
  }
}

CEREAL_REGISTER_TYPE(hop::FitnessFunctionGrid)
