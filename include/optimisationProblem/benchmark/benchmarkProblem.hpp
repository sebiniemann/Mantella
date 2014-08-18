#pragma once

#include <optimisationProblem/optimisationProblem.hpp>

namespace hop {
  class BenchmarkProblem : public OptimisationProblem {
    public:
      BenchmarkProblem(const unsigned int& numberOfDimensions);
      
    protected:
      const Col<double> _randomTranslation;

      Col<double> getRandomParameterTranslation(const Col<double>& parameter) const;
      Col<double> getScaling(const double& condition) const;
      Col<double> getAsymmetricTransformation(const double& beta, const Col<double>& parameter) const;
      double getOscillationTransformation(const double& value) const;
      Col<double> getOscillationTransformation(const Col<double>& parameter) const;
      double getPenality(const Col<double>& parameter) const;
  };
}
