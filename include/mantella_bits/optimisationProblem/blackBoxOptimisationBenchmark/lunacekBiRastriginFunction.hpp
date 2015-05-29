namespace mant {
  namespace bbob {
    template <typename T = double>
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark<T> {
      static_assert(std::is_floating_point<T>::value, "T must be a floating point type.");
    
      public:
        explicit LunacekBiRastriginFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationR(
            const arma::Mat<T>& rotationR);

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const T s_;
        const T mu_;
        const arma::Col<T> parameterConditinong_;

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
          archive(cereal::make_nvp("parameterRotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LunacekBiRastriginFunction>& construct) noexcept {
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
    LunacekBiRastriginFunction<T>::LunacekBiRastriginFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T>(numberOfDimensions),
        s_(static_cast<T>(1.0L) - static_cast<T>(0.5L) / (std::sqrt(static_cast<T>(this->numberOfDimensions_) + static_cast<T>(20.0L)) - static_cast<T>(4.1L))),
        mu_(std::sqrt(static_cast<T>(5.25L) / s_)),
        parameterConditinong_(this->getParameterConditioning(static_cast<T>(10.0L))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      this->setParameterScaling(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? static_cast<T>(2.0L) : static_cast<T>(-2.0L)));
      setRotationR(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T>
    void LunacekBiRastriginFunction<T>::setRotationR(
        const arma::Mat<T>& rotationR) {
      verify(rotationR.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    template <typename T>
    void LunacekBiRastriginFunction<T>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T>
    T LunacekBiRastriginFunction<T>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<T>(10000.0L) * this->getBoundConstraintsValue(parameter);
    }

    template <typename T>
    T LunacekBiRastriginFunction<T>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return std::min(std::pow(arma::norm(parameter - static_cast<T>(2.5L)), static_cast<T>(2.0L)), static_cast<T>(this->numberOfDimensions_) + s_ * std::pow(arma::norm(parameter + mu_), static_cast<T>(2.0L))) + static_cast<T>(10.0L) * (static_cast<T>(this->numberOfDimensions_) - arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * rotationQ_ * (parameterConditinong_ % (rotationR_ * (parameter - static_cast<T>(2.5L)))))));
    }

    template <typename T>
    std::string LunacekBiRastriginFunction<T>::toString() const noexcept {
      return "bbob_lunacek_bi_rastrigin_function";
    }
  }
}
