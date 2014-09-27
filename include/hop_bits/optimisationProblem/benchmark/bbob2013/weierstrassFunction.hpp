#pragma once

#include <cmath>

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class WeierstrassFunction : public BlackBoxOptimisationBenchmark2013 {
    public:
      explicit WeierstrassFunction(const unsigned int& numberOfDimensions);

      WeierstrassFunction(const WeierstrassFunction&) = delete;
      WeierstrassFunction& operator=(const WeierstrassFunction&) = delete;

      std::string to_string() const override;

    protected:
      double f0_;
      const arma::Col<double> delta_ = getScaling(std::sqrt(0.01));

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("translation", translation_));
        archive(cereal::make_nvp("rotationR", rotationR_));
        archive(cereal::make_nvp("rotationQ", rotationQ_));
        archive(cereal::make_nvp("f0", f0_));
      }

      template<class T>
      static void load_and_construct(T& archive, cereal::construct<WeierstrassFunction>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("translation", construct->translation_));
        archive(cereal::make_nvp("rotationR", construct->rotationR_));
        archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        archive(cereal::make_nvp("f0", construct->f0_));
      }
  };
}
