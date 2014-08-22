#pragma once

#include <optimisationProblem/optimisationProblem.hpp>

namespace hop {
  class BenchmarkProblem : public OptimisationProblem {
    public:
      BenchmarkProblem(const unsigned int& numberOfDimensions);
      
    protected:
      Col<double> _translation;
      Col<double> _one;
      Mat<double> _rotationR;
      Mat<double> _rotationQ;
      Mat<double> _deltaC101;
      Mat<double> _localOptimaY101;
      Mat<double> _deltaC21;
      Mat<double> _localOptimaY21;

      void setTranslation(const Col<double>& translation);
      void setOne(const Col<double>& one);
      void setRotationR(const Mat<double>& rotationR);
      void setRotationQ(const Mat<double>& rotationQ);
      void setDeltaC101(const Mat<double>& deltaC101);
      void setLocalOptimaY101(const Mat<double>& localOptimaY101);
      void setDeltaC21(const Mat<double>& deltaC21);
      void setLocalOptimaY21(const Mat<double>& localOptimaY21);

      Mat<double> getRandomRotation() const;
      Mat<double> getRandomDeltaC101() const;
      Mat<double> getRandomLocalOptimaY101() const;
      Mat<double> getRandomDeltaC21() const;
      Mat<double> getRandomLocalOptimaY21() const;
      Col<double> getScaling(const double& condition) const;
      Col<double> getScaling(const Col<double>& condition) const;
      Col<double> getAsymmetricTransformation(const double& beta, const Col<double>& parameter) const;
      double getOscillationTransformation(const double& value) const;
      Col<double> getOscillationTransformation(const Col<double>& parameter) const;
      double getPenality(const Col<double>& parameter) const;
  };
}
