namespace mant {
  namespace bbob2009 {
    class AttractiveSectorFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        AttractiveSectorFunction(const AttractiveSectorFunction&) = delete;
        AttractiveSectorFunction& operator=(const AttractiveSectorFunction&) = delete;

        inline std::string to_string() const  override;

      protected:
        const arma::Col<double> delta_ = getScaling(std::sqrt(10.0));

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const  override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<AttractiveSectorFunction>& construct) {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    inline double AttractiveSectorFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const  {
      arma::Col<double> z = rotationQ_ * (delta_ % (rotationR_ * (parameter - translation_)));
      z.elem(arma::find(z % translation_ > 0.0)) *= 100.0;

      return std::pow(getOscillationTransformation(std::pow(norm(z), 2.0)), 0.9);
    }

    inline std::string AttractiveSectorFunction::to_string() const  {
      return "AttractiveSectorFunction";
    }
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::AttractiveSectorFunction);
#endif
