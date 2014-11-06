#pragma once

// C++ Standard Library
#include <algorithm>
#include <cmath>

// HOP
#include <hop_bits/optimisationProblem/benchmark/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  namespace bbob2013 {
    class RosenbrockFunction : public BlackBoxOptimisationBenchmark2013 {
      public:
        explicit RosenbrockFunction(
            const unsigned int& numberOfDimensions);

        RosenbrockFunction(const RosenbrockFunction&) = delete;
        RosenbrockFunction& operator=(const RosenbrockFunction&) = delete;

        void setTranslation(
            const arma::Col<double>& translation) override;

        std::string to_string() const override;

      protected:
        const double max_ = std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0);

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const override;

        friend class cereal::access;

        template<class T>
        void serialize(T& archive) {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
        }

        template<class T>
        static void load_and_construct(T& archive, cereal::construct<RosenbrockFunction>& construct) {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
        }
    };
  }
}
