#pragma once

// Cereal
#include <cereal/types/base_class.hpp>

// HOP
#include <hop_bits/optimisationProblem.hpp>

namespace hop {
  namespace bbob2012 {
    class BlackBoxOptimisationBenchmark2012 : public OptimisationProblem<double> {
      public:
        explicit BlackBoxOptimisationBenchmark2012(
            const unsigned int& numberOfDimensions) noexcept;

        BlackBoxOptimisationBenchmark2012(const BlackBoxOptimisationBenchmark2012&) = delete;
        BlackBoxOptimisationBenchmark2012& operator=(const BlackBoxOptimisationBenchmark2012&) = delete;

        virtual void setTranslation(
            const arma::Col<double>& translation);
        virtual void setOne(
            const arma::Col<double>& one);
        virtual void setRotationR(
            const arma::Mat<double>& rotationR);
        virtual void setRotationQ(
            const arma::Mat<double>& rotationQ);
        virtual void setDeltaC101(
            const arma::Mat<double>& deltaC101);
        virtual void setLocalOptimaY101(
            const arma::Mat<double>& localOptimaY101);
        virtual void setDeltaC21(
            const arma::Mat<double>& deltaC21);
        virtual void setLocalOptimaY21(
            const arma::Mat<double>& localOptimaY21);

        virtual ~BlackBoxOptimisationBenchmark2012() = default;

      protected:
        arma::Col<double> translation_;
        arma::Col<double> one_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;
        arma::Mat<double> deltaC101_;
        arma::Mat<double> localOptimaY101_;
        arma::Mat<double> deltaC21_;
        arma::Mat<double> localOptimaY21_;

        arma::Mat<double> getRandomDeltaC101() const noexcept;
        arma::Mat<double> getRandomLocalOptimaY101() const noexcept;
        arma::Mat<double> getRandomDeltaC21() const noexcept;
        arma::Mat<double> getRandomLocalOptimaY21() const noexcept;
        arma::Col<double> getScaling(
            const double& condition) const noexcept;
        arma::Col<double> getScaling(
            const arma::Col<double>& condition) const noexcept;
        arma::Col<double> getAsymmetricTransformation(
            const double& beta, const arma::Col<double>& parameter) const noexcept;
        double getOscillationTransformation(
            const double& value) const noexcept;
        arma::Col<double> getOscillationTransformation(
            const arma::Col<double>& parameter) const noexcept;
        double getPenality(
            const arma::Col<double>& parameter) const noexcept;

        friend class cereal::access;
        BlackBoxOptimisationBenchmark2012() = default;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("optimisationProblem", cereal::base_class<OptimisationProblem>(this)));
          archive(cereal::make_nvp("translation", translation_));
        }
    };
  }
}
