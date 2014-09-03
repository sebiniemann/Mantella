#pragma once

#include <memory>

#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class OptimisationAlgorithm {
    public:
      OptimisationAlgorithm();
      arma::Col<double> optimise(std::shared_ptr<OptimisationProblem> optimisationProblem);

      bool isFinished() const;

    protected:
      std::shared_ptr<OptimisationProblem> _optimisationProblem;

      arma::Col<double> _bestParameter;
      double _bestObjectiveValue;

      virtual void initialisation();
      virtual arma::Mat<double> getNextParameters() = 0;
      virtual void updateInformations(arma::Mat<double> parameters, arma::Mat<double> objectiveValues, arma::Mat<double> softConstraintsValues);

      bool isTerminated() const;
  };
}
