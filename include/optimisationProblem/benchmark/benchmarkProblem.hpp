#pragma once

#include <cereal/types/base_class.hpp>

#include <optimisationProblem/optimisationProblem.hpp>

namespace hop {
  class BenchmarkProblem : public OptimisationProblem {
    public:
      BenchmarkProblem(const unsigned int& numberOfDimensions);

      virtual void setTranslation(const arma::Col<double>& translation);
      virtual void setOne(const arma::Col<double>& one);
      virtual void setRotationR(const arma::Mat<double>& rotationR);
      virtual void setRotationQ(const arma::Mat<double>& rotationQ);
      virtual void setDeltaC101(const arma::Mat<double>& deltaC101);
      virtual void setLocalOptimaY101(const arma::Mat<double>& localOptimaY101);
      virtual void setDeltaC21(const arma::Mat<double>& deltaC21);
      virtual void setLocalOptimaY21(const arma::Mat<double>& localOptimaY21);
      
    protected:
      arma::Col<double> _translation;
      arma::Col<double> _one;
      arma::Mat<double> _rotationR;
      arma::Mat<double> _rotationQ;
      arma::Mat<double> _deltaC101;
      arma::Mat<double> _localOptimaY101;
      arma::Mat<double> _deltaC21;
      arma::Mat<double> _localOptimaY21;

      arma::Mat<double> getRandomRotation() const;
      arma::Mat<double> getRandomDeltaC101() const;
      arma::Mat<double> getRandomLocalOptimaY101() const;
      arma::Mat<double> getRandomDeltaC21() const;
      arma::Mat<double> getRandomLocalOptimaY21() const;
      arma::Col<double> getScaling(const double& condition) const;
      arma::Col<double> getScaling(const arma::Col<double>& condition) const;
      arma::Col<double> getAsymmetricTransformation(const double& beta, const arma::Col<double>& parameter) const;
      double getOscillationTransformation(const double& value) const;
      arma::Col<double> getOscillationTransformation(const arma::Col<double>& parameter) const;
      double getPenality(const arma::Col<double>& parameter) const;

      friend class cereal::access;
      BenchmarkProblem() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("optimisationProblem", cereal::base_class<OptimisationProblem>(this)));
      }
  };
}
