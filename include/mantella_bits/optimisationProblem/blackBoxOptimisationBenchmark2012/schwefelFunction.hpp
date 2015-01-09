#pragma once

// C++ Standard Library
#include <cmath>

// Mantella
#include <mantella_bits/optimisationProblem/blackBoxOptimisationBenchmark2012.hpp>

namespace mant {
  namespace bbob2012 {
    class SchwefelFunction : public BlackBoxOptimisationBenchmark2012 {
      public:
        using BlackBoxOptimisationBenchmark2012::BlackBoxOptimisationBenchmark2012;

        SchwefelFunction(const SchwefelFunction&) = delete;
        SchwefelFunction& operator=(const SchwefelFunction&) = delete;

        std::string to_string() const noexcept override;

      protected:
        arma::Col<double> delta_ = getScaling(std::sqrt(10));

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("one", one_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SchwefelFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2012", cereal::base_class<BlackBoxOptimisationBenchmark2012>(construct.ptr())));
          archive(cereal::make_nvp("one", construct->one_));
        }
    };
  }
}
