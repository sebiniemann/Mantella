#pragma once

// C++ Standard Library
#include <algorithm>
#include <cmath>

// Mantella
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012.hpp>

namespace mant {
  namespace bbob2012 {
    class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark2012 {
      public:
        using BlackBoxOptimisationBenchmark2012::BlackBoxOptimisationBenchmark2012;

        RosenbrockFunctionRotated(const RosenbrockFunctionRotated&) = delete;
        RosenbrockFunctionRotated& operator=(const RosenbrockFunctionRotated&) = delete;

        std::string to_string() const noexcept override;

      protected:
        const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RosenbrockFunctionRotated>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
    };
  }
}
