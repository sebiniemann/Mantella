namespace mant {
  namespace bbob2009 {
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        LunacekBiRastriginFunction(const LunacekBiRastriginFunction&) = delete;
        LunacekBiRastriginFunction& operator=(const LunacekBiRastriginFunction&) = delete;

        inline std::string to_string() const noexcept override;

      protected:
        const arma::Col<double> delta_ = getScaling(std::sqrt(100.0));
        const double s_ = 1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1);
        const double mu1_ = -std::sqrt(5.25 / s_);

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("one", one_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LunacekBiRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("one", construct->one_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline double LunacekBiRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& xHat = 2.0 * arma::sign(one_) % parameter;
      const arma::Col<double>& z = rotationQ_ * (delta_ % (rotationR_ * (xHat - 2.5)));

      return std::min(arma::accu(arma::square(xHat - 2.5)), static_cast<double>(numberOfDimensions_) + s_ * arma::accu(arma::square(xHat - mu1_))) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + 10000.0 * getPenality(parameter);
    }

    inline std::string LunacekBiRastriginFunction::to_string() const noexcept {
      return "LunacekBiRastriginFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::LunacekBiRastriginFunction);
#endif
