namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class RastriginFunctionRotated : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
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
    RastriginFunctionRotated<T, U>::RastriginFunctionRotated(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        parameterConditioning_(this->getParameterConditioning(std::sqrt(static_cast<T>(10.0L)))) {
      this->setParameterTranslation(this->getRandomParameterTranslation());
      setRotationR(getRandomRotationMatrix(this->numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
    }

    template <typename T, typename U>
    void RastriginFunctionRotated<T, U>::setRotationR(
        const arma::Mat<T>& rotationR) {
      verify(rotationR.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationR), "The parameter must be a rotation matrix.");

      rotationR_ = rotationR;
    }

    template <typename T, typename U>
    void RastriginFunctionRotated<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    U RastriginFunctionRotated<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      const arma::Col<T>& z = rotationR_ * (parameterConditioning_ % (rotationQ_ * this->getAsymmetricParameter(static_cast<T>(0.2L), this->getOscillatedParameter(rotationR_ * parameter))));

      return static_cast<U>(10.0L) * (static_cast<U>(this->numberOfDimensions_) - static_cast<U>(arma::accu(arma::cos(static_cast<T>(2.0L) * arma::datum::pi * z)))) + std::pow(static_cast<U>(arma::norm(z)), static_cast<U>(2.0L));
    }

    template <typename T, typename U>
    std::string RastriginFunctionRotated<T, U>::toString() const noexcept {
      return "bbob_rastrigin_function_rotated";
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> RastriginFunctionRotated<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationQ_(n)));
      }
      
      for(std::size_t n = 0; n < rotationR_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationR_(n)));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void RastriginFunctionRotated<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
      
      rotationR_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationR_.n_elem; ++n) {
        rotationR_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}
