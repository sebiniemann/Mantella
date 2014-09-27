#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      BuecheRastriginFunction(const BuecheRastriginFunction&) = delete;
      BuecheRastriginFunction& operator=(const BuecheRastriginFunction&) = delete;

      std::string to_string() const override;

      void setTranslation(const arma::Col<double>& translation) override;

    protected:
      const arma::Col<double> scaling_ = getScaling(std::sqrt(10.0));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;
      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("translation", translation_));
      }

      template<class T>
      static void load_and_construct(T& archive, cereal::construct<BuecheRastriginFunction>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("translation", construct->translation_));
      }
  };
}
