namespace mant {
  namespace bbob2009 {
    class EllipsoidalFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        EllipsoidalFunction(const EllipsoidalFunction&) = delete;
        EllipsoidalFunction& operator=(const EllipsoidalFunction&) = delete;

        inline std::string to_string() const  override;

      protected:
        const arma::Col<double> scaling_ = getScaling(1000000.0);

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        template <typename Archive>
        void serialize(Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<EllipsoidalFunction>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
        }
#endif
    };

    inline double EllipsoidalFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      return arma::dot(scaling_, arma::square(getOscillationTransformation(parameter - translation_)));
    }

    inline std::string EllipsoidalFunction::to_string() const  {
      return "EllipsoidalFunction";
    }
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::EllipsoidalFunction);
#endif
