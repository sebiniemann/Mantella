namespace mant {
  namespace bbob {
    template <typename T = double, typename U = double>
    class GallaghersGaussian101mePeaksFunction : public BlackBoxOptimisationBenchmark<T, U> {
      static_assert(std::is_floating_point<T>::value, "The parameter type T must be a floating point type.");
      static_assert(std::is_floating_point<U>::value, "The codomain type U must be a floating point type.");
    
      public:
        explicit GallaghersGaussian101mePeaksFunction(
            const std::size_t numberOfDimensions) noexcept;

        virtual void setRotationQ(
            const arma::Mat<T>& rotationQ);

        virtual void setLocalParameterConditionings(
            const arma::Mat<T>& localParameterConditionings);

        virtual void setLocalParameterTranslations(
            const arma::Mat<T>& localParameterTranslations);

        std::string toString() const noexcept override;

      protected:
        const typename arma::Col<U>::template fixed<101> weight_;

        arma::Mat<T> rotationQ_;
        arma::Mat<T> localParameterConditionings_;
        arma::Mat<T> localParameterTranslations_;

        arma::Mat<T> getRandomLocalParameterConditionings() const noexcept;

        arma::Mat<T> getRandomLocalParameterTranslations() const noexcept;

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
    GallaghersGaussian101mePeaksFunction<T, U>::GallaghersGaussian101mePeaksFunction(
        const std::size_t numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark<T, U>(numberOfDimensions),
        weight_(arma::join_cols(arma::Col<U>({static_cast<U>(10.0L)}), arma::linspace<arma::Col<U>>(static_cast<U>(1.1L), static_cast<U>(9.1L), 100))) {
      setRotationQ(getRandomRotationMatrix(this->numberOfDimensions_));
      setLocalParameterConditionings(getRandomLocalParameterConditionings());
      setLocalParameterTranslations(getRandomLocalParameterTranslations());
    }

    template <typename T, typename U>
    void GallaghersGaussian101mePeaksFunction<T, U>::setRotationQ(
        const arma::Mat<T>& rotationQ) {
      verify(rotationQ.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(isRotationMatrix(rotationQ), "The parameter must be a rotation matrix.");

      rotationQ_ = rotationQ;
    }

    template <typename T, typename U>
    void GallaghersGaussian101mePeaksFunction<T, U>::setLocalParameterConditionings(
        const arma::Mat<T>& localParameterConditionings) {
      verify(localParameterConditionings.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterConditionings.n_cols == 101, "The number of columns must be equal to the number of peaks (101).");

      localParameterConditionings_ = localParameterConditionings;
    }

    template <typename T, typename U>
    void GallaghersGaussian101mePeaksFunction<T, U>::setLocalParameterTranslations(
        const arma::Mat<T>& localParameterTranslations) {
      verify(localParameterTranslations.n_rows == this->numberOfDimensions_, "The number of rows must be equal to the number of dimensions");
      verify(localParameterTranslations.n_cols == 101, "The number of columns must be equal to the number of peaks (101).");

      localParameterTranslations_ = localParameterTranslations;
    }

    template <typename T, typename U>
    arma::Mat<T> GallaghersGaussian101mePeaksFunction<T, U>::getRandomLocalParameterConditionings() const noexcept {
      arma::Col<T> conditions(101);
      conditions(0) = static_cast<T>(49.5L);
      conditions.tail(conditions.n_elem - 1) = arma::conv_to<arma::Col<T>>::from(getRandomPermutation(conditions.n_elem - 1));

      arma::Mat<T> localParameterConditionings(this->numberOfDimensions_, conditions.n_elem);
      for (std::size_t n = 0; n < conditions.n_elem; ++n) {
        const T& condition = std::pow(static_cast<T>(10.0L), conditions(n) / static_cast<T>(33.0L));
        localParameterConditionings.col(n) = this->getParameterConditioning(condition) / std::sqrt(condition);
      }

      return localParameterConditionings;
    }

    template <typename T, typename U>
    arma::Mat<T> GallaghersGaussian101mePeaksFunction<T, U>::getRandomLocalParameterTranslations() const noexcept {
      arma::Mat<T> localParameterTranslations = arma::randu<arma::Mat<T>>(this->numberOfDimensions_, 101) * static_cast<T>(10.0L) - static_cast<T>(5.0L);
      localParameterTranslations.col(0) = static_cast<T>(0.8L) * localParameterTranslations.col(0);

      return localParameterTranslations;
    }

    template <typename T, typename U>
    U GallaghersGaussian101mePeaksFunction<T, U>::getSoftConstraintsValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      return this->getBoundConstraintsValue(parameter);
    }

    template <typename T, typename U>
    U GallaghersGaussian101mePeaksFunction<T, U>::getObjectiveValueImplementation(
        const arma::Col<T>& parameter) const noexcept {
      U maximalValue = std::numeric_limits<U>::lowest();
      for (std::size_t k = 0; k < 101; ++k) {
        const arma::Col<T>& locallyTranslatedParameter = parameter - localParameterTranslations_.col(k);
        maximalValue = std::max(maximalValue, weight_(k) * std::exp(static_cast<U>(-0.5L) / static_cast<U>(this->numberOfDimensions_) * static_cast<U>(arma::dot(locallyTranslatedParameter, rotationQ_.t() * arma::diagmat(localParameterConditionings_.col(k)) * rotationQ_ * locallyTranslatedParameter))));
      }

      return std::pow(this->getOscillatedObjectiveValue(static_cast<U>(10.0L) - maximalValue), static_cast<U>(2.0L));
    }

    template <typename T, typename U>
    std::string GallaghersGaussian101mePeaksFunction<T, U>::toString() const noexcept {
      return "bbob_gallaghers_gaussian_101me_peaks_function";
    }

#if defined(MANTELLA_USE_PARALLEL_ALGORITHMS)
    template <typename T, typename U>
    std::vector<double> GallaghersGaussian101mePeaksFunction<T, U>::serialise() const noexcept {
      std::vector<double> serialisedOptimisationProblem = BlackBoxOptimisationBenchmark<T, T>::serialise();
      
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(rotationQ_(n)));
      }
      
      for(std::size_t n = 0; n < localParameterConditionings_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(localParameterConditionings_(n)));
      }
      
      for(std::size_t n = 0; n < localParameterTranslations_.n_elem; ++n) {
        serialisedOptimisationProblem.push_back(static_cast<double>(localParameterTranslations_(n)));
      }
      
      return serialisedOptimisationProblem;
    }

    template <typename T, typename U>
    void GallaghersGaussian101mePeaksFunction<T, U>::deserialise(
        const std::vector<double>& serialisedOptimisationProblem) {
      rotationQ_.set_size(this->numberOfDimensions_, this->numberOfDimensions_);
      for(std::size_t n = 0; n < rotationQ_.n_elem; ++n) {
        rotationQ_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
      
      localParameterConditionings_.set_size(this->numberOfDimensions_, 21);
      for(std::size_t n = 0; n < localParameterConditionings_.n_elem; ++n) {
        localParameterConditionings_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
      
      localParameterTranslations_.set_size(this->numberOfDimensions_, 21);
      for(std::size_t n = 0; n < localParameterTranslations_.n_elem; ++n) {
        localParameterTranslations_(n) = static_cast<T>(serialisedOptimisationProblem.pop_back());
      }
        
      BlackBoxOptimisationBenchmark<T, T>::deserialise(serialisedOptimisationProblem);
    }
#endif
  }
}