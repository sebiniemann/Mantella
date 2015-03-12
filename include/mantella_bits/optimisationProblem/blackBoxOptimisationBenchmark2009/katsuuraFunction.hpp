namespace mant {
  namespace bbob2009 {
    class KatsuuraFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit KatsuuraFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> scaling_;

        arma::Col<double> localParameterTranslation_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
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
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline KatsuuraFunction::KatsuuraFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(10.0)) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void KatsuuraFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      checkDimensionCompatible("The number of elements", localParameterTranslation.n_elem, "the number of dimensions", numberOfDimensions_);

      localParameterTranslation_ = localParameterTranslation;
    }

    inline void KatsuuraFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      checkDimensionCompatible("The number of rows", rotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationR);

      rotationR_ = rotationR;
    }

    inline void KatsuuraFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      checkDimensionCompatible("The number of rows", rotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationQ);

      rotationQ_ = rotationQ;
    }

    inline double KatsuuraFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = rotationQ_ * (scaling_ % (rotationR_ * (parameter - localParameterTranslation_)));

      double product = 1.0;
      for (std::size_t n = 0; n < z.n_elem; ++n) {
          const double& value = z(n);

          double sum = 0.0;
          for (unsigned int k = 1; k < 33; ++k) {
              const double& power = std::pow(2.0, k);
              sum += std::abs(power * value - std::round(power * value)) / power;
          }

          product *= std::pow(1.0 + static_cast<double>(n + 1) * sum, (10.0 / std::pow(numberOfDimensions_, 1.2)));
      }

      return 10.0 / std::pow(numberOfDimensions_, 2.0) * (product - 1.0) + getPenality(parameter);
    }

    inline std::string KatsuuraFunction::toString() const noexcept {
      return "katsuura-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::KatsuuraFunction);
#endif
