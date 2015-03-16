namespace mant {
  namespace bbob2009 {
    class KatsuuraFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit KatsuuraFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setParameterRotationR(
            const arma::Mat<double>& parameterRotationR);

        inline void setParameterRotationQ(
            const arma::Mat<double>& parameterRotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        arma::Mat<double> parameterRotationR_;
        arma::Mat<double> parameterRotationQ_;

        inline double getSoftConstraintsValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("parameterRotationR", parameterRotationR_));
          archive(cereal::make_nvp("parameterRotationQ", parameterRotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<KatsuuraFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->parameterRotationR_));
          archive(cereal::make_nvp("parameterRotationQ", construct->parameterRotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline KatsuuraFunction::KatsuuraFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(10.0)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setParameterRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void KatsuuraFunction::setParameterRotationR(
        const arma::Mat<double>& parameterRotationR) {
      checkDimensionCompatibility("The number of rows", parameterRotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", parameterRotationR);

      parameterRotationR_ = parameterRotationR;
    }

    inline void KatsuuraFunction::setParameterRotationQ(
        const arma::Mat<double>& parameterRotationQ) {
      checkDimensionCompatibility("The number of rows", parameterRotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", parameterRotationQ);

      parameterRotationQ_ = parameterRotationQ;
    }

    inline double KatsuuraFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return getBoundConstraintsValue(parameter);
    }

    inline double KatsuuraFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameterRotationQ_ * (parameterConditioning_ % (parameterRotationR_ * parameter));

      double product = 1.0;
      for (std::size_t n = 0; n < z.n_elem; ++n) {
          const double& value = z(n);

          double sum = 0.0;
          for (unsigned int k = 1; k < 33; ++k) {
              const double& power = std::pow(2.0, k);
              sum += std::abs(power * value - std::round(power * value)) / power;
          }

          product *= std::pow(1.0 + (static_cast<double>(n) + 1.0) * sum, (10.0 / std::pow(static_cast<double>(numberOfDimensions_), 1.2)));
      }

      return 10.0 / std::pow(static_cast<double>(numberOfDimensions_), 2.0) * (product - 1.0);
    }

    inline std::string KatsuuraFunction::toString() const noexcept {
      return "katsuura-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::KatsuuraFunction);
#endif
