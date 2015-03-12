namespace mant {
  namespace bbob2009 {
    class GallaghersGaussian21hiPeaksFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit GallaghersGaussian21hiPeaksFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline virtual void setRotationR(
            const arma::Mat<double>& rotationR);

        inline virtual void setLocalParameterScaling(
            const arma::Col<double>& localParameterScaling);

        inline virtual void setLocalOptima(
            const arma::Mat<double>& localOptima);

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> weight_;

        arma::Mat<double> rotationR_;
        arma::Col<double> localParameterScaling_;
        arma::Mat<double> localOptima_;

        inline arma::Col<double> getRandomLocalParameterScaling() const noexcept;

        inline arma::Mat<double> getRandomLocalOptima() const noexcept;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("localParameterScaling", localParameterScaling_));
          archive(cereal::make_nvp("localOptima", localOptima_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<GallaghersGaussian21hiPeaksFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("localParameterScaling", construct->localParameterScaling_));
          archive(cereal::make_nvp("localOptima", construct->localOptima_));
        }
#endif
    };

    //
    // Implementation
    //

    inline GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        weight_(21),
        localParameterScaling_(21) {
      weight_(0) = 10.0;
      for (std::size_t n = 1; n < weight_.n_elem; ++n) {
        weight_(n) = 1.1 + 8.0 * static_cast<double>(n - 1) / 19.0;
      }

      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setLocalParameterScaling(getRandomLocalParameterScaling());
      setLocalOptima(getRandomLocalOptima());
    }

    inline void GallaghersGaussian21hiPeaksFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      checkDimensionCompatible("The number of rows", rotationR.n_rows, "the number of dimensions", numberOfDimensions_);
      checkRotationMatrix("The matrix", rotationR);

      rotationR_ = rotationR;
    }

    inline void GallaghersGaussian21hiPeaksFunction::setLocalParameterScaling(
        const arma::Col<double>& localParameterScaling) {
      checkDimensionCompatible("The number of elements", localParameterScaling.n_elem, "the number of peaks", 21);

      for (std::size_t n = 0; n < localParameterScaling.n_elem; ++n) {
        const double& localParameterScalingValue = std::pow(10.0, localParameterScaling(n) / 33.0);
        localParameterScaling_.col(n) = getScaledTransformation(localParameterScalingValue) / std::sqrt(localParameterScalingValue);
      }
    }

    inline void GallaghersGaussian21hiPeaksFunction::setLocalOptima(
        const arma::Mat<double>& localOptima) {
      checkDimensionCompatible("The number of rows", localOptima.n_rows, "the number of dimensions", numberOfDimensions_);
      checkDimensionCompatible("The number of columns", localOptima.n_cols, "the number of peaks", 21);

      localOptima_ = localOptima;
    }

    inline arma::Col<double> GallaghersGaussian21hiPeaksFunction::getRandomLocalParameterScaling() const noexcept {
      arma::Col<double> localParameterScaling(21);
      localParameterScaling(0) = 9.5;
      localParameterScaling.tail(20) = arma::conv_to<arma::Col<double>>::from(getRandomPermutation(20));

      return localParameterScaling;
    }

    inline arma::Mat<double> GallaghersGaussian21hiPeaksFunction::getRandomLocalOptima() const noexcept {
      arma::Mat<double> localOptima = arma::randu<arma::Mat<double>>(numberOfDimensions_, 21) * 9.8 - 4.9;
      localOptima.col(0) = 0.8 * localOptima.col(0);

      return localOptima;
    }

    inline double GallaghersGaussian21hiPeaksFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      double maximalValue = std::numeric_limits<double>::lowest();
      for (unsigned int k = 0; k < 21; ++k) {
        const arma::Col<double>& localParameterTranslation = parameter - localOptima_.col(k);
        maximalValue = std::max(maximalValue, weight_(k) * std::exp(-1.0 / (2.0 * static_cast<double>(numberOfDimensions_)) * arma::dot(localParameterTranslation, rotationR_.t() * arma::diagmat(localParameterScaling_.col(k)) * rotationR_ * localParameterTranslation)));
      }

      return std::pow(getOscillatedTransformation(10.0 - maximalValue), 2.0) + getPenality(parameter);
    }

    inline std::string GallaghersGaussian21hiPeaksFunction::toString() const noexcept {
      return "gallaghers-gaussian-21hi-peaks-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::GallaghersGaussian21hiPeaksFunction);
#endif
