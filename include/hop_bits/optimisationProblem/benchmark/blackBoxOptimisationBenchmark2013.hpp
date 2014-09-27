#pragma once

#include <cereal/types/base_class.hpp>

#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  class BlackBoxOptimisationBenchmark2013 : public OptimisationProblem {
    public:
      explicit BlackBoxOptimisationBenchmark2013(const unsigned int& numberOfDimensions);

      BlackBoxOptimisationBenchmark2013(const BlackBoxOptimisationBenchmark2013&) = delete;
      BlackBoxOptimisationBenchmark2013& operator=(const BlackBoxOptimisationBenchmark2013&) = delete;

      virtual void setTranslation(const arma::Col<double>& translation);
      virtual void setOne(const arma::Col<double>& one);
      virtual void setRotationR(const arma::Mat<double>& rotationR);
      virtual void setRotationQ(const arma::Mat<double>& rotationQ);
      virtual void setDeltaC101(const arma::Mat<double>& deltaC101);
      virtual void setLocalOptimaY101(const arma::Mat<double>& localOptimaY101);
      virtual void setDeltaC21(const arma::Mat<double>& deltaC21);
      virtual void setLocalOptimaY21(const arma::Mat<double>& localOptimaY21);
      
    protected:
      arma::Col<double> translation_;
      arma::Col<double> one_;
      arma::Mat<double> rotationR_;
      arma::Mat<double> rotationQ_;
      arma::Mat<double> deltaC101_;
      arma::Mat<double> localOptimaY101_;
      arma::Mat<double> deltaC21_;
      arma::Mat<double> localOptimaY21_;

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
      BlackBoxOptimisationBenchmark2013() = default;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("optimisationProblem", cereal::base_class<OptimisationProblem>(this)));
        archive(cereal::make_nvp("translation", translation_));
      }
  };
}
