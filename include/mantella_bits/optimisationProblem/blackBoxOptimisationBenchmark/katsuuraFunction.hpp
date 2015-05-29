namespace mant {
  namespace bbob {
    template <typename T = double>
    class KatsuuraFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit KatsuuraFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

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

    template <typename T>
    KatsuuraFunction<T>::KatsuuraFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(static_cast<T>(10.0L))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void KatsuuraFunction<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T KatsuuraFunction<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return this->getBoundConstraintsValue(parameter);
    }

    template <typename T>
    T KatsuuraFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      arma::Col<T> z = rotationQ_ * (parameterConditioning_ % parameter);

      T product = static_cast<T>(1.0L);
      for (std::size_t n = 0; n < z.n_elem; ++n) {
          const T& value = z(n);

          T sum = static_cast<T>(0.0L);
          for (std::size_t k = 1; k < 33; ++k) {
              const T& power = std::pow(static_cast<T>(2.0L), k);
              sum += std::abs(power * value - std::round(power * value)) / power;
          }

          product *= std::pow(static_cast<T>(1.0L) + (static_cast<T>(n) + static_cast<T>(1.0L)) * sum, (static_cast<T>(10.0L) / std::pow(static_cast<T>(this->numberOfDimensions_), static_cast<T>(1.2L))));
      }

      return static_cast<T>(10.0L) / std::pow(static_cast<T>(this->numberOfDimensions_), static_cast<T>(2.0L)) * (product - static_cast<T>(1.0L));
    }

    template <typename T>
    std::string KatsuuraFunction<T>::toString() const noexcept {
      return "bbob_katsuura_function";
    }
  }
}
