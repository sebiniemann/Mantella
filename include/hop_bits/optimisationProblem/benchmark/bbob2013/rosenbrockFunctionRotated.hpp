#pragma once

#include <algorithm>
#include <cmath>

#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark2013 {
    public:
      using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

      RosenbrockFunctionRotated(const RosenbrockFunctionRotated&) = delete;
      RosenbrockFunctionRotated& operator=(const RosenbrockFunctionRotated&) = delete;

      std::string to_string() const override;

    protected:
      const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

      double getObjectiveValueImplementation(const arma::Col<double>& parameter) const override;

      friend class cereal::access;

      template<class T>
      void serialize(T& archive) {
        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        archive(cereal::make_nvp("rotationR", rotationR_));
      }

      template<class T>
      static void load_and_construct(T& archive, cereal::construct<RosenbrockFunctionRotated>& construct) {
        unsigned int numberOfDimensions;
        archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
        construct(numberOfDimensions);

        archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
        archive(cereal::make_nvp("rotationR", construct->rotationR_));
      }
  };
}
