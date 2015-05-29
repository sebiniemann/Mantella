namespace mant {
  namespace bbob {
    template <typename T = double>
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit WeierstrassFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationR(
            const arma::Mat<T>& rotationR);

        void setRotationQ(
            const arma::Mat<T>& rotationQ);
            
        std::string toString() const noexcept override;

      protected:
        const T f0_;
        const arma::Col<T> parameterConditioning_;

        arma::Mat<T> rotationR_;
        arma::Mat<T> rotationQ_;

        T getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

        T getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

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

    template <typename T>
    WeierstrassFunction<T>::WeierstrassFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        f0_(static_cast<T>(-1.99951171875L)),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(0.01L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void WeierstrassFunction<T>::setRotationR(
        const arma::Mat<T>& rotationR) {
      verify(rotationR.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    template <typename T>
    void WeierstrassFunction<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T WeierstrassFunction<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<T>(10.0L) * this->getBoundConstraintsValue(parameter) / static_cast<T>(this->numberOfDimensions_);
    }
    
    template <typename T>
    T WeierstrassFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * this->getOscillatedParameter(rotationR_ * parameter)));

      T sum = static_cast<T>(0.0L);
      for (std::size_t n = 0; n < z.n_elem; ++n) {
        for (std::size_t k = 0; k < 12; ++k) {
          sum += std::pow(static_cast<T>(0.5L), k) * std::cos(static_cast<T>(2.0L) * arma::datum::pi * std::pow(static_cast<T>(3.0L), k) * (z(n) + static_cast<T>(0.5L)));
        }
      }

      return static_cast<T>(10.0L) * std::pow(sum / static_cast<T>(this->numberOfDimensions_) - f0_, static_cast<T>(3.0L));
    }

    template <typename T>
    std::string WeierstrassFunction<T>::toString() const noexcept {
      return "bbob_weierstrass_function";
    }
  }
}
