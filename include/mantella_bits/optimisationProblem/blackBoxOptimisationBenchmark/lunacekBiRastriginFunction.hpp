namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit LunacekBiRastriginFunction(
            const std::size_t numberOfDimensions) noexcept;

        void setRotationR(
            const arma::Mat<T>& rotationR);

        void setRotationQ(
            const arma::Mat<T>& rotationQ);

        std::string toString() const noexcept override;

      protected:
        const U s_;
        const U mu_;
        const arma::Col<T> parameterConditinong_;

        arma::Mat<T> rotationR_;
        arma::Mat<T> rotationQ_;

        U getSoftConstraintsValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;

        U getObjectiveValueImplementation(
            const arma::Col<T>& parameter) const noexcept override;
        
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
      // Grants direct access to the otherwise hidden .serialise() and .deserialise(...) methods.
      friend class OptimisationAlgorithm;

      // The type is intentionally fixed to ease usage with MPI_DOUBLE.
      std::vector<double> serialise() const noexcept;

      // The type is intentionally fixed to ease usage with MPI_DOUBLE.
      void deserialise(
          const std::vector<double>& serialisedOptimisationProblem);
#endif
    };

    //
    // Implementation
    //

    template <typename T, typename U>
    LunacekBiRastriginFunction<T, U>::LunacekBiRastriginFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        s_(static_cast<U>(1.0L) - static_cast<U>(0.5L) / (std::sqrt(static_cast<U>(this->numberOfDimensions_) + static_cast<U>(20.0L)) - static_cast<U>(4.1L))),
        mu_(std::sqrt(static_cast<U>(5.25L) / s_)),
        parameterConditinong_(this->getParameterConditioning(static_cast<T>(10.0L))) {
      // A vector with all elements randomly and uniformly set to either 2 or -2.
      this->setParameterScaling(arma::zeros<arma::Col<T>>(this->numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? static_cast<T>(2.0L) : static_cast<T>(-2.0L)));
      setRotationR(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    void LunacekBiRastriginFunction<T, U>::setRotationR(
        const arma::Mat<T>& rotationR) {
      verify(rotationR.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    template <typename T, typename U>
    void LunacekBiRastriginFunction<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    U LunacekBiRastriginFunction<T, U>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<U>(10000.0L) * this->getBoundConstraintsValue(parameter);
    }

    template <typename T, typename U>
    U LunacekBiRastriginFunction<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return std::min(std::pow(static_cast<U>(arma::norm(parameter - static_cast<T>(2.5L))), static_cast<U>(2.0L)), static_cast<U>(this->numberOfDimensions_) + s_ * std::pow(static_cast<U>(arma::norm(parameter + mu_)), static_cast<U>(2.0L))) + static_cast<U>(10.0L) * (static_cast<U>(this->numberOfDimensions_) - static_cast<U>(arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * rotationQ_ * (parameterConditinong_ % (rotationR_ * (parameter - static_cast<T>(2.5L))))))));
    }

    template <typename T, typename U>
    std::string LunacekBiRastriginFunction<T, U>::toString() const noexcept {
      return "bbob_lunacek_bi_rastrigin_function";
    }
    
#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> LunacekBiRastriginFunction<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationQ_(n)));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void LunacekBiRastriginFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
