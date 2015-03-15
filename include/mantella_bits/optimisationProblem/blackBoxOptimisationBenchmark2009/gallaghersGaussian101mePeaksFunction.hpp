namespace mant {
  namespace bbob2009 {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit GallaghersGaussian101mePeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline virtual void setParameterRotationR(
            const arma::Mat<double>& parameterRotationR);

        inline virtual void setLocalParameterConditioning(
            const arma::Col<double>& localParameterConditioning);

        inline virtual void setLocalParameterTranslation(
            const arma::Mat<double>& localParameterTranslation);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double>::fixed<101> weight_;

        arma::Mat<double> parameterRotationR_;
        arma::Mat<double> localParameterConditioning_;
        arma::Mat<double> localParameterTranslation_;

        inline arma::Col<double> getRandomLocalParameterConditioning() const noexcept;

        inline arma::Mat<double> getRandomLocalParameterTranslation() const noexcept;

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
          archive(cereal::make_nvp("localParameterConditioning", localParameterConditioning_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian101mePeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->parameterRotationR_));
          archive(cereal::make_nvp("localParameterConditioning", construct->localParameterConditioning_));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        weight_(arma::join_cols(arma::Col<double>({10}), arma::linspace<arma::Col<double>>(1.1, 9.1, 100))) {
      setParameterRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterConditioning(getRandomLocalParameterConditioning());
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
    }

    inline void GallaghersGaussian101mePeaksFunction::setParameterRotationR(
        const arma::Mat<double>& parameterRotationR) {
      checkDimensionCompatible("The number of rows", parameterRotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", parameterRotationR);

      parameterRotationR_ = parameterRotationR;
    }

    inline void GallaghersGaussian101mePeaksFunction::setLocalParameterConditioning(
        const arma::Col<double>& localParameterConditioning) {
      checkDimensionCompatible("The number of elements", localParameterConditioning.n_elem, "the number of peaks", 101);

      localParameterConditioning_.set_size(numberOfDimensions_, 101);
      for (std::size_t n = 0; n < localParameterConditioning.n_elem; ++n) {
        const double& localParameterConditioningValue = std::pow(10.0, localParameterConditioning(n) / 33.0);
        localParameterConditioning_.col(n) = getParameterConditioning(localParameterConditioningValue) / std::sqrt(localParameterConditioningValue);
      }
    }

    inline void GallaghersGaussian101mePeaksFunction::setLocalParameterTranslation(
        const arma::Mat<double>& localParameterTranslation) {
      checkDimensionCompatible("The number of rows", localParameterTranslation.n_rows, "the number of dimensions", numberOfDimensions_);
      checkDimensionCompatible("The number of columns", localParameterTranslation.n_cols, "the number of peaks", 101);

      localParameterTranslation_ = localParameterTranslation;
    }

    inline arma::Col<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterConditioning() const noexcept {
      arma::Col<double> localParameterConditioning(101);
      localParameterConditioning(0) = 49.5;
      localParameterConditioning.tail(100) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(100));

      return localParameterConditioning;
    }

    inline arma::Mat<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterTranslation() const noexcept {
      arma::Mat<double> localParameterTranslation = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 10.0 - 5.0;
      localParameterTranslation.col(0) = 0.8 * localParameterTranslation.col(0);

      return localParameterTranslation;
    }

    inline double GallaghersGaussian101mePeaksFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return getBoundConstraintsValue(parameter);
    }

    inline double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 101; ++k) {
        const arma::Col<double>& locallyTranslatedParameter = parameter - localParameterTranslation_.col(k);
        maximalValue = std::max(maximalValue, weight_(k) * std::exp(-0.5 / static_cast<double>(numberOfDimensions_) * arma::dot(locallyTranslatedParameter, parameterRotationR_.t() * arma::diagmat(localParameterConditioning_.col(k)) * parameterRotationR_ * locallyTranslatedParameter)));
      }

      return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
    }

    inline std::string GallaghersGaussian101mePeaksFunction::toString() const noexcept {
      return "gallaghers-gaussian-101me-peaks-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob2009::GallaghersGaussian101mePeaksFunction);
#endif
