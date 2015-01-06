#pragma once

// C++ Standard Library
#include <cmath>

// HOP
#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2015.hpp>

namespace hop {
  namespace bbob2015 {
    class RastriginFunctionRotated : public BlackBoxOptimisationBenchmark2015 {
      public:
        using BlackBoxOptimisationBenchmark2015::BlackBoxOptimisationBenchmark2015;

        RastriginFunctionRotated(const RastriginFunctionRotated&) = delete;
        RastriginFunctionRotated& operator=(const RastriginFunctionRotated&) = delete;

        std::string to_string() const noexcept override;

      protected:
        const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2015", cereal::base_class<BlackBoxOptimisationBenchmark2015>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RastriginFunctionRotated>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2015", cereal::base_class<BlackBoxOptimisationBenchmark2015>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
    };
  }
}
