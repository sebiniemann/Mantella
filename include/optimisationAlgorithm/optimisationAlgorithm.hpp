#pragma once

#include <memory>

#include <optimisationProblem/optimisationProblem.hpp>

namespace hop {
  class OptimisationAlgorithm{
    public:
      OptimisationAlgorithm();
      virtual arma::Col<double> optimise(std::shared_ptr<OptimisationProblem> optimisationProblem) final;

      virtual bool isFinished() const final;

    protected:
      std::shared_ptr<OptimisationProblem> _optimisationProblem;

      arma::Col<double> _bestParameter;
      double _bestObjectiveValue;

      virtual void initialisation();
      virtual arma::Mat<double> getNextParameters() = 0;
      virtual void updateInformations(arma::Mat<double> parameters, arma::Mat<double> objectiveValues, arma::Mat<double> softConstraintsValues);

      virtual bool isTerminated() const final;
  };
}
