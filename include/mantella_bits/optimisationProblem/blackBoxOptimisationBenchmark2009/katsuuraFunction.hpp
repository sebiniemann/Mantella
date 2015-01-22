namespace mant {
  namespace bbob2009 {
    class KatsuuraFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        KatsuuraFunction(const KatsuuraFunction&) = delete;
        KatsuuraFunction& operator=(const KatsuuraFunction&) = delete;

        inline std::string to_string() const noexcept override;

      protected:
        const arma::Col<double> delta_ = getScaling(std::sqrt(100.0));

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<KatsuuraFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline double KatsuuraFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));

      double product = 1.0;
      for (std::size_t n = 0; n < z.n_elem; ++n) {
          const double& value = z.at(n);

          double sum = 0.0;
          for (unsigned int k = 1; k < 33; ++k) {
              const double& power = std::pow(2.0, k);
              sum += std::abs(power * value - std::round(power * value)) / power;
          }

          product *= std::pow(1.0 + static_cast<double>(n + 1) * sum, (10.0 / std::pow(numberOfDimensions_, 1.2)));
      }

      return 10.0 / std::pow(numberOfDimensions_, 2.0) * (product - 1.0) + getPenality(parameter);
    }

    inline std::string KatsuuraFunction::to_string() const noexcept {
      return "KatsuuraFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::KatsuuraFunction);
#endif
