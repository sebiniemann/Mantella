namespace mant {
  namespace bbob {
    class DiscusFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit DiscusFunction(
            const unsigned int numberOfDimensions) noexcept;

        inline std::string toString() const noexcept override;

      protected:
        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<DiscusFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
        }
#endif
    };

    //
    // Implementation
    //

    inline DiscusFunction::DiscusFunction(
        const unsigned int numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setParameterTranslation(getRandomParameterTranslation());
      setParameterRotation(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline double DiscusFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = arma::square(getOscillatedParameter(parameter));
      return 1000000.0 * z(0) + arma::accu(z.tail(z.n_elem - 1));
    }

    inline std::string DiscusFunction::toString() const noexcept {
      return "bbob_discus_function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
CEREAL_REGISTER_TYPE(mant::bbob::DiscusFunction);
#endif
