namespace mant {
  namespace bbob2009 {
    class BentCigarFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        BentCigarFunction(const BentCigarFunction&) = delete;
        BentCigarFunction& operator=(const BentCigarFunction&) = delete;

        inline std::string to_string() const  override;

      protected:
        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive)  {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<BentCigarFunction>& construct)  {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
#endif
    };

    inline double BentCigarFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      const arma::Col<double>& z = arma::square(rotationR_ * getAsymmetricTransformation(0.5, rotationR_ * (parameter - translation_)));
      return z.at(0) + 1000000.0 * arma::accu(z.tail(z.n_elem - 1));
    }

    inline std::string BentCigarFunction::to_string() const  {
      return "BentCigarFunction";
    }
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::BentCigarFunction);
#endif
