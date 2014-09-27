#pragma once

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class DiscusFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      DiscusFunction(const DiscusFunction&) = delete;
      DiscusFunction& operator=(const DiscusFunction&) = delete;

      std::string to_string() const override;

    protected:
      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("translation", translation_));
        archive(cereal::make_nvp("rotationR", rotationR_));
      }

      template<class T>
      static void load_and_construct(T& archive, cereal::construct<DiscusFunction>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("translation", construct->translation_));
        archive(cereal::make_nvp("rotationR", construct->rotationR_));
      }
  };
}
