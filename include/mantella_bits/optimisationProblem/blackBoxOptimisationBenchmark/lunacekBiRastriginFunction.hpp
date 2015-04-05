namespace mant {
  namespace bbob {
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit LunacekBiRastriginFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setParameterRotationR(
            const arma::Mat<double>& parameterRotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const double s_;
        const double mu_;
        const arma::Col<double> parameterConditinong_;

        arma::Mat<double> parameterRotationR_;
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
          archive(cereal::make_nvp("parameterRotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LunacekBiRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->parameterRotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline LunacekBiRastriginFunction::LunacekBiRastriginFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        s_(1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1)),
        mu_(std::sqrt(5.25 / s_) + 2.5),
        parameterConditinong_(getParameterConditioning(10.0)) {
      // A vector with al elements set to 2.5.
      setParameterTranslation(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 2.5);
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      setParameterScaling(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 2.0 : -2.0));
      setParameterRotationR(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void LunacekBiRastriginFunction::setParameterRotationR(
        const arma::Mat<double>& parameterRotationR) {
      isEqual("The number of rows", parameterRotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", parameterRotationR);

      parameterRotationR_ = parameterRotationR;
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void LunacekBiRastriginFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      isEqual("The number of rows", rotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", rotationQ);

      rotationQ_ = rotationQ;
    }

    inline double LunacekBiRastriginFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return 10000.0 * getBoundConstraintsValue(parameter);
    }

    inline double LunacekBiRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = parameterRotationQ_ * (parameterConditinong_ % (parameterRotationR_ * parameter));

      return std::min(std::pow(arma::norm(parameter), 2.0), static_cast<double>(numberOfDimensions_) + s_ * std::pow(arma::norm(parameter + mu_), 2.0)) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z)));
    }

    inline std::string LunacekBiRastriginFunction::toString() const noexcept {
      return "bbob_lunacek_bi_rastrigin_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::LunacekBiRastriginFunction);
#endif
