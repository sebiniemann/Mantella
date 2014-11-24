#pragma once

// C++ Standard Library
#include <algorithm>
#include <cmath>

// HOP
#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  namespace bbob2013 {
    class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark2013 {
      public:
        using BlackBoxOptimisationBenchmark2013::BlackBoxOptimisationBenchmark2013;

        RosenbrockFunctionRotated(const RosenbrockFunctionRotated&) = delete;
        RosenbrockFunctionRotated& operator=(const RosenbrockFunctionRotated&) = delete;

        std::string to_string() const noexcept override;

      protected:
        const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template<class Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template<class Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RosenbrockFunctionRotated>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
    };
  }
}
