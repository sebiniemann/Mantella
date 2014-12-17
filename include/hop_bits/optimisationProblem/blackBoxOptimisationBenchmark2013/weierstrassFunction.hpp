#pragma once

// C++ Standard Library
#include <cmath>

// HOP
#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  namespace bbob2013 {
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark2013 {
      public:
        explicit WeierstrassFunction(
            const unsigned int& numberOfDimensions) noexcept;

        WeierstrassFunction(const WeierstrassFunction&) = delete;
        WeierstrassFunction& operator=(const WeierstrassFunction&) = delete;

        std::string to_string() const noexcept override;

      protected:
        double f0_;
        const arma::Col<double> delta_ = getScaling(std::sqrt(0.01));

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
          archive(cereal::make_nvp("f0", f0_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<WeierstrassFunction>& construct) noexcept {
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
}
