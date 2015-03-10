namespace mant {
  namespace bbob2009 {
    class SphereFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit SphereFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> localParameterTranslation_;
        
        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SphereFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
        }
#endif
    };

    //
    // Implementation
    //

    inline SphereFunction::SphereFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
    }

    inline void SphereFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      if (localParameterTranslation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the local translation (" + std::to_string(localParameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      localParameterTranslation_ = localParameterTranslation;
    }

    inline double SphereFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return std::pow(arma::norm(parameter - localParameterTranslation_), 2.0);
    }

    inline std::string SphereFunction::toString() const noexcept {
      return "sphere-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::SphereFunction);
#endif
