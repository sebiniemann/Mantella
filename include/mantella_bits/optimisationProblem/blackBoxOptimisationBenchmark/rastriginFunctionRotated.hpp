namespace mant {
  namespace bbob {
    template <typename T = double>
    class RastriginFunctionRotated : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit RastriginFunctionRotated(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationR(
            const arma::Mat<T>& rotationR);

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const arma::Col<T> parameterConditioning_;

        arma::Mat<T> rotationR_;
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
          archive(cereal::make_nvp("parameterRotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RastriginFunctionRotated>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark", cereal::base_class<BlackBoxOptimisationBenchmark>(construct.ptr())));
          archive(cereal::make_nvp("parameterRotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    template <typename T>
    RastriginFunctionRotated<T>::RastriginFunctionRotated(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void RastriginFunctionRotated<T>::setRotationR(
        const arma::Mat<T>& rotationR) {
      verify(rotationR.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    template <typename T>
    void RastriginFunctionRotated<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T RastriginFunctionRotated<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * this->getAsymmetricParameter(static_cast<T>(0.2L), this->getOscillatedParameter(rotationR_ * parameter))));

      return static_cast<T>(10.0L) * (static_cast<T>(this->numberOfDimensions_) - arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * z))) + std::pow(arma::norm(z), static_cast<T>(2.0L));
    }

    template <typename T>
    std::string RastriginFunctionRotated<T>::toString() const noexcept {
      return "bbob_rastrigin_function_rotated";
    }
  }
}
