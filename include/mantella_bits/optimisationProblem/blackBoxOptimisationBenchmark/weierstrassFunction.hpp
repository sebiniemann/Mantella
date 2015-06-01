namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
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

    template <typename T, typename U>
    WeierstrassFunction<T, U>::WeierstrassFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        f0_(static_cast<T>(-1.99951171875L)),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(0.01L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    void WeierstrassFunction<T, U>::setRotationR(
        const arma::Mat<T>& rotationR) {
      verify(rotationR.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    template <typename T, typename U>
    void WeierstrassFunction<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    T WeierstrassFunction<T, U>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return static_cast<T>(10.0L) * this->getBoundConstraintsValue(parameter) / static_cast<T>(this->numberOfDimensions_);
    }
    
    template <typename T, typename U>
    T WeierstrassFunction<T, U>::getObjectiveValueImplementation(
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

    template <typename T, typename U>
    std::string WeierstrassFunction<T, U>::toString() const noexcept {
      return "bbob_weierstrass_function";
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> WeierstrassFunction<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationQ_(n));
      }
      
      for(std::size_t n = 0; n < rotationR_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(rotationR_(n));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void WeierstrassFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = serialisedOptimisationProblem.pop_back();
      }
      
      rotationR_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationR_.n_elem; ++n) {
        rotationR_(n) = serialisedOptimisationProblem.pop_back();
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
