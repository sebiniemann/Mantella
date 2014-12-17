#pragma once

// C++ Standard Library
#include <cmath>

// HOP
#include <hop_bits/optimisationProblem/blackBoxOptimisationBenchmark2013.hpp>

namespace hop {
  namespace bbob2013 {
    class BuecheRastriginFunction : public BlackBoxOptimisationBenchmark2013 {
      public:
        explicit BuecheRastriginFunction(
            const unsigned int& numberOfDimensions) noexcept;

        BuecheRastriginFunction(const BuecheRastriginFunction&) = delete;
        BuecheRastriginFunction& operator=(const BuecheRastriginFunction&) = delete;

        std::string to_string() const noexcept override;

        void setTranslation(
            const arma::Col<double>& translation) override;

      protected:
        const arma::Col<double> scaling_ = getScaling(std::sqrt(10.0));

        double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        friend class cereal::access;
        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<BuecheRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2013", cereal::base_class<BlackBoxOptimisationBenchmark2013>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
        }
    };
  }
}
