namespace mant {
  namespace bbob {
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark {
      public:
        inline explicit WeierstrassFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);
            
        inline std::string toString() const noexcept override;

      protected:
        const double f0_;
        const arma::Col<double> parameterConditioning_;

        arma::Mat<double> rotationR_;
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
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<WeierstrassFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline WeierstrassFunction::WeierstrassFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark(numberOfDimensions),
        f0_(-1.99951171875),
        parameterConditioning_(getParameterConditioning(std::sqrt(0.01))) {
      setParameterTranslation(getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void WeierstrassFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      verify(rotationR.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    inline void WeierstrassFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      verify(rotationQ.n_rows == numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    inline double WeierstrassFunction::getSoftConstraintsValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return 10.0 * getBoundConstraintsValue(parameter) / static_cast<double>(numberOfDimensions_);
    }
    
    inline double WeierstrassFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * getOscillatedParameter(rotationR_ * parameter)));

      double sum = 0.0;
      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        for (unsigned int k = 0; k < 12; ++k) {
          sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z(n) + 0.5));
        }
      }

      return 10 * std::pow(sum / static_cast<double>(numberOfDimensions_) - f0_, 3);
    }

    inline std::string WeierstrassFunction::toString() const noexcept {
      return "bbob_weierstrass_function";
    }
  }
}
