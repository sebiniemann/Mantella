namespace mant {
  namespace bbob {
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit GallaghersGaussian101mePeaksFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline virtual void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline virtual void setLocalParameterConditionings(
            const arma::Mat<double>& localParameterConditionings);

        inline virtual void setLocalParameterTranslations(
            const arma::Mat<double>& localParameterTranslations);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double>::fixed<101> weight_;

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
            cereal::construct<GallaghersGaussian101mePeaksFunction>& construct) noexcept {
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

    inline GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        weight_(arma::join_cols(arma::Col<double>({10}), arma::linspace<arma::Col<double>>(1.1, 9.1, 100))) {
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterConditionings(getRandomLocalParameterConditionings());
      setLocalParameterTranslations(getRandomLocalParameterTranslations());
    }

    inline void GallaghersGaussian101mePeaksFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    inline void GallaghersGaussian101mePeaksFunction::setLocalParameterConditionings(
        const arma::Mat<double>& localParameterConditionings) {
      verify(localParameterConditionings.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterConditionings.n_cols == 101, "The number of columns must be equal to the number of peaks (101).");

      localParameterConditionings_ = localParameterConditionings;
    }

    inline void GallaghersGaussian101mePeaksFunction::setLocalParameterTranslations(
        const arma::Mat<double>& localParameterTranslations) {
      verify(localParameterTranslations.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterTranslations.n_cols == 101, "The number of columns must be equal to the number of peaks (101).");

      localParameterTranslations_ = localParameterTranslations;
    }

    inline arma::Mat<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterConditionings() const noexcept {
      arma::Col<double> conditions(101);
      conditions(0) = 49.5;
      conditions.tail(conditions.n_elem - 1) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(conditions.n_elem - 1));

      arma::Mat<double> localParameterConditionings(numberOfDimensions_, conditions.n_elem);
      for (std::size_t n = 0; n < conditions.n_elem; ++n) {
        const double& condition = std::pow(10.0, conditions(n) / 33.0);
        localParameterConditionings.col(n) = getParameterConditioning(condition) / std::sqrt(condition);
      }

      return localParameterConditionings;
    }

    inline arma::Mat<double> GallaghersGaussian101mePeaksFunction::getRandomLocalParameterTranslations() const noexcept {
      arma::Mat<double> localParameterTranslations = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 10.0 - 5.0;
      localParameterTranslations.col(0) = 0.8 * localParameterTranslations.col(0);

      return localParameterTranslations;
    }

    inline double GallaghersGaussian101mePeaksFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return getBoundConstraintsValue(parameter);
    }

    inline double GallaghersGaussian101mePeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 101; ++k) {
        const arma::Col<double>& locallyTranslatedParameter = parameter - localParameterTranslations_.col(k);
        maximalValue = std::max(maximalValue, weight_(k) * std::exp(-0.5 / static_cast<double>(numberOfDimensions_) * arma::dot(locallyTranslatedParameter, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(k)) * rotationQ_ * locallyTranslatedParameter)));
      }

      return std::pow(getOscillatedValue(10.0 - maximalValue), 2.0);
    }

    inline std::string GallaghersGaussian101mePeaksFunction::toString() const noexcept {
      return "bbob_gallaghers_gaussian_101me_peaks_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::GallaghersGaussian101mePeaksFunction);
#endif
