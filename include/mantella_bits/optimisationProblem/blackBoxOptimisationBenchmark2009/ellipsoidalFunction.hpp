namespace mant {
  namespace bbob2009 {
    class EllipsoidalFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit EllipsoidalFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> scaling_;

        arma::Col<double> localParameterTranslation_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<EllipsoidalFunction>& construct) noexcept {
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

    inline EllipsoidalFunction::EllipsoidalFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(1000000.0)) {
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
    }

    inline void EllipsoidalFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      if (localParameterTranslation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the local translation (" + std::to_string(localParameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      localParameterTranslation_ = localParameterTranslation;
    }


    inline double EllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      return arma::dot(scaling_, arma::square(getOscillatedTransformation(parameter - localParameterTranslation_)));
    }

    inline std::string EllipsoidalFunction::toString() const noexcept {
      return "ellipsoidal-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::EllipsoidalFunction);
#endif
