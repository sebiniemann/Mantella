#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      LunacekBiRastriginFunction(const LunacekBiRastriginFunction&) = delete;
      LunacekBiRastriginFunction& operator=(const LunacekBiRastriginFunction&) = delete;

      std::string to_string() const override;

    protected:
      const arma::Col<double> delta_ = getScaling(std::sqrt(100.0));
      const double s_ = 1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1);
      const double mu1_ = -std::sqrt((6.25 - 1) / s_);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("one", one_));
        archive(cereal::make_nvp("rotationR", rotationR_));
        archive(cereal::make_nvp("rotationQ", rotationQ_));
      }

      template<class T>
      static void load_and_construct(T& archive, cereal::construct<LunacekBiRastriginFunction>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("one", construct->one_));
        archive(cereal::make_nvp("rotationR", construct->rotationR_));
        archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
      }
  };
}
