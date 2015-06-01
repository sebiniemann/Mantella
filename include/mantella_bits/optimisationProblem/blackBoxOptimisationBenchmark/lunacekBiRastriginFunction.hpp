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
        
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
        friend class OptimisationAlgorithm;
        
        std::vector<double> serialise() const noexcept;

        void deserialise(
            const std::vector<double>& serialisedOptimisationProblem);
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
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T>
    std::vector<double> LunacekBiRastriginFunction<T>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T>
    void LunacekBiRastriginFunction<T>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.pop_back();
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
