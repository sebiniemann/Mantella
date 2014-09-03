#include <plotter/fitnessFunctionGridPlotter.hpp>

#include <armadillo>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include <script/fitnessFunctionGrid.hpp>

FitnessFunctionGridPlotter::FitnessFunctionGridPlotter(const std::shared_ptr<hop::OptimisationProblem> optimisationProblem) {
  _optimisationProblem = optimisationProblem;
}

void FitnessFunctionGridPlotter::runImpelementation() {
  FitnessFunctionGrid fitnessFunctionGrid(_optimisationProblem);
  fitnessFunctionGrid.run();

  plot("fitnessFunctionGrid");
}

CEREAL_REGISTER_TYPE(FitnessFunctionGridPlotter)
