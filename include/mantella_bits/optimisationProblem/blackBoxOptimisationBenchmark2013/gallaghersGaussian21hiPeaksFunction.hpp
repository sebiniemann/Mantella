#pragma once

// Mantella
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2013.hpp>

namespace mant {
  namespace bbob2013 {
    class GallaghersGaussian21hiPeaksFunction : public BlackBoxOptimisationBenchmark2013 {
      public:
        explicit GallaghersGaussian21hiPeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        GallaghersGaussian21hiPeaksFunction(const GallaghersGaussian21hiPeaksFunction&) = delete;
        GallaghersGaussian21hiPeaksFunction& operator=(const GallaghersGaussian21hiPeaksFunction&) = delete;

        std::string to_string() const noexcept override;

      protected:
        arma::Col<double> weight_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("deltaC21", deltaC21_));
          archive(cereal::make_nvp("localOptimaY21", localOptimaY21_));
          archive(cereal::make_nvp("weight", weight_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian21hiPeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("deltaC21", construct->deltaC21_));
          archive(cereal::make_nvp("localOptimaY21", construct->localOptimaY21_));
          archive(cereal::make_nvp("weight", construct->weight_));
        }
    };
  }
}

