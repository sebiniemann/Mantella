#pragma once

// Mantella
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012.hpp>

namespace mant {
  namespace bbob2012 {
    class DifferentPowersFunction : public BlackBoxOptimisationBenchmark2012 {
      public:
        using BlackBoxOptimisationBenchmark2012::BlackBoxOptimisationBenchmark2012;

        DifferentPowersFunction(const DifferentPowersFunction&) = delete;
        DifferentPowersFunction& operator=(const DifferentPowersFunction&) = delete;

        std::string to_string() const noexcept override;

      protected:
        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<DifferentPowersFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
    };
  }
}

