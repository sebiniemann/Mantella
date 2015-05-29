namespace mant {
  namespace bbob {
    template <typename T = double>
    class SharpRidgeFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit SharpRidgeFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        arma::Mat<T> rotationQ_;

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
            cereal::construct<SharpRidgeFunction>& construct) noexcept {
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
    SharpRidgeFunction<T>::SharpRidgeFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      this->setParameterRotation(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void SharpRidgeFunction<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T SharpRidgeFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = rotationQ_ * (parameterConditioning_ %  parameter);
      return std::pow(z(0), static_cast<T>(2.0L)) + static_cast<T>(100.0L) * arma::norm(z.tail(z.n_elem - 1));
    }

    template <typename T>
    std::string SharpRidgeFunction<T>::toString() const noexcept {
      return "bbob_sharp_ridge_function";
    }
  }
}
