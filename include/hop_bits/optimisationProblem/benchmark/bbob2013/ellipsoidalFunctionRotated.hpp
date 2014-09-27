#pragma once

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class EllipsoidalFunctionRotated : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      EllipsoidalFunctionRotated(const EllipsoidalFunctionRotated&) = delete;
      EllipsoidalFunctionRotated& operator=(const EllipsoidalFunctionRotated&) = delete;

      std::string to_string() const override;

    protected:
      const arma::Col<double> scaling_ = getScaling(1000000.0);

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
      static void load_and_construct(T& archive, cereal::construct<EllipsoidalFunctionRotated>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("translation", construct->translation_));
        archive(cereal::make_nvp("rotationR", construct->rotationR_));
      }
  };
}
