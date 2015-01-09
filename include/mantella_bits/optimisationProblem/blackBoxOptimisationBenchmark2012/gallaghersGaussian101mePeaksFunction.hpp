#pragma once

// Mantella
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012.hpp>

namespace mant {
  namespace bbob2012 {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2012 {
      public:
        explicit GallaghersGaussian101mePeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        GallaghersGaussian101mePeaksFunction(const GallaghersGaussian101mePeaksFunction&) = delete;
        GallaghersGaussian101mePeaksFunction& operator=(const GallaghersGaussian101mePeaksFunction&) = delete;

        std::string to_string() const noexcept override;

      protected:
        arma::Col<double> weight_;

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("deltaC101", deltaC101_));
          archive(cereal::make_nvp("localOptimaY101", localOptimaY101_));
          archive(cereal::make_nvp("weight", weight_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian101mePeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("deltaC101", construct->deltaC101_));
          archive(cereal::make_nvp("localOptimaY101", construct->localOptimaY101_));
          archive(cereal::make_nvp("weight", construct->weight_));
        }
    };
  }
}
