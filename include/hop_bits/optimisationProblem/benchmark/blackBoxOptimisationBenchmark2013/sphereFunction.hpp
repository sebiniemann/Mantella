#pragma once

// HOP
#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  namespace bbob2013 {
    class SphereFunction : public BlackBoxOptimisationBenchmark2013 {
      public:
        using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

        SphereFunction(const SphereFunction&) = delete;
        SphereFunction& operator=(const SphereFunction&) = delete;

        std::string to_string() const noexcept override;

      protected:
        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template<class Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
        }

        template<class Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SphereFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
        }
    };
  }
}
