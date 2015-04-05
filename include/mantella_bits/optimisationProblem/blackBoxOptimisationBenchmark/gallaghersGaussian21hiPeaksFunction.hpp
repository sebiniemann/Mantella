namespace mant {
  namespace bbob {
    class GallaghersGaussian21hiPeaksFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit GallaghersGaussian21hiPeaksFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline virtual void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline virtual void setLocalParameterConditionings(
            const arma::Mat<double>& localParameterConditionings);

        inline virtual void setLocalParameterTranslations(
            const arma::Mat<double>& localParameterTranslations);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double>::fixed<21> weight_;

        arma::Mat<double> rotationQ_;
        arma::Mat<double> localParameterConditionings_;
        arma::Mat<double> localParameterTranslations_;

        inline arma::Mat<double> getRandomLocalParameterConditionings() const noexcept;

        inline arma::Mat<double> getRandomLocalParameterTranslations() const noexcept;

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
          archive(cereal::make_nvp("localParameterConditionings", localParameterConditionings_));
          archive(cereal::make_nvp("localParameterTranslations", localParameterTranslations_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian21hiPeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
          archive(cereal::make_nvp("localParameterConditionings", construct->localParameterConditionings_));
          archive(cereal::make_nvp("localParameterTranslations", construct->localParameterTranslations_));
        }
#endif
    };

    //
    // Implementation
    //

    inline GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        weight_(arma::join_cols(arma::Col<double>({10}), arma::linspace<arma::Col<double>>(1.1, 9.1, 20))) {
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterConditionings(getRandomLocalParameterConditionings());
      setLocalParameterTranslations(getRandomLocalParameterTranslations());
    }

    inline void GallaghersGaussian21hiPeaksFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      isEqual("The number of rows", rotationQ.n_rows, "the number of dimensions", numberOfDimensions_);
      isRotationMatrix("The matrix", rotationQ);

      rotationQ_ = rotationQ;
    }

    inline void GallaghersGaussian21hiPeaksFunction::setLocalParameterConditionings(
        const arma::Mat<double>& localParameterConditionings) {
      isEqual("The number of rows", localParameterConditionings.n_rows, "the number of dimensions", numberOfDimensions_);
      isEqual("The number of columns", localParameterConditionings.n_cols, "the number of peaks", 21);

      localParameterConditionings_ = localParameterConditionings;
    }

    inline void GallaghersGaussian21hiPeaksFunction::setLocalParameterTranslations(
        const arma::Mat<double>& localParameterTranslations) {
      isEqual("The number of rows", localParameterTranslations.n_rows, "the number of dimensions", numberOfDimensions_);
      isEqual("The number of columns", localParameterTranslations.n_cols, "the number of peaks", 21);

      localParameterTranslations_ = localParameterTranslations;
    }

    inline arma::Mat<double> GallaghersGaussian21hiPeaksFunction::getRandomLocalParameterConditionings() const noexcept {
      arma::Col<double> conditions(21);
      conditions(0) = 19.0;
      conditions.tail(conditions.n_elem - 1) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(conditions.n_elem - 1));

      arma::Mat<double> localParameterConditionings(numberOfDimensions_, conditions.n_elem);
      for (std::size_t n = 0; n < conditions.n_elem; ++n) {
        const double& condition = std::pow(1000.0, conditions(n) / 19.0);
        localParameterConditionings.col(n) = getParameterConditioning(condition) / std::sqrt(condition);
      }

      return localParameterConditionings;
    }

    inline arma::Mat<double> GallaghersGaussian21hiPeaksFunction::getRandomLocalParameterTranslations() const noexcept {
      arma::Mat<double> localParameterTranslations = arma::randu<arma::Mat<double>>(numberOfDimensions_, 21) * 9.8 - 4.9;
      localParameterTranslations.col(0) = 0.8 * localParameterTranslations.col(0);

      return localParameterTranslations;
    }

    inline double GallaghersGaussian21hiPeaksFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return getBoundConstraintsValue(parameter);
    }

    inline double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 21; ++k) {
        const arma::Col<double>& locallyTranslatedParameter = parameter - localParameterTranslations_.col(k);
        maximalValue = std::max(maximalValue, weight_(k) * std::exp(-0.5 / static_cast<double>(numberOfDimensions_) * arma::dot(locallyTranslatedParameter, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(k)) * rotationQ_ * locallyTranslatedParameter)));
      }

      return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
    }

    inline std::string GallaghersGaussian21hiPeaksFunction::toString() const noexcept {
      return "bbob_gallaghers_gaussian_21hi_peaks_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::GallaghersGaussian21hiPeaksFunction);
#endif
