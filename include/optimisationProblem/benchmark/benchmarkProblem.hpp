#pragma once

#include <armadillo>
using arma::Col;

#include <optimisationProblem/optimisationProblem.hpp>

namespace hop {
  class BenchmarkProblem : public OptimisationProblem {
    public:
      BenchmarkProblem(const unsigned int& numberOfDimensions);
      
    protected:
      Col<double> getScaling(const double& condition) const;
      Col<double> getAsymmetricTransformation(const double& beta, const Col<double>& parameter) const;
      double getOscillationTransformation(const double& value) const;
      Col<double> getOscillationTransformation(const Col<double>& parameter) const;
      double getPenality(const Col<double>& parameter) const;
  };
}
