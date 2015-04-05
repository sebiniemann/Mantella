namespace mant {
  namespace bbob {
    class KatsuuraFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit KatsuuraFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> parameterConditioning_;

        arma::Mat<double> rotationQ_;

        inline double getSoftConstraintsValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<KatsuuraFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline KatsuuraFunction::KatsuuraFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        parameterConditioning_(getParameterConditioning(10.0)) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void KatsuuraFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      isEqual("The number of rows", rotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", rotationQ);

      rotationQ_ = rotationQ;
    }

    inline double KatsuuraFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return getBoundConstraintsValue(parameter);
    }

    inline double KatsuuraFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = rotationQ_ * (parameterConditioning_ % parameter);

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
      return "bbob_katsuura_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::KatsuuraFunction);
#endif
