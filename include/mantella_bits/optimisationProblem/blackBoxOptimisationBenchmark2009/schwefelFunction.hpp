namespace mant {
  namespace bbob2009 {
    class SchwefelFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        using BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009;

        inline std::string to_string() const noexcept override;

      protected:
        arma::Col<double> delta_ = getScaling(std::sqrt(10));

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("one", one_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<SchwefelFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("one", construct->one_));
        }
#endif
    };

    //
    // Implementation
    //

    inline double SchwefelFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& xOpt = arma::abs(4.2096874633 * one_);
      const arma::Col<double>& xHat = 2.0 * one_ % parameter;

      arma::Col<double> zHat(xHat.n_elem);
      zHat.at(0) = xHat.at(0);
      zHat.tail(zHat.n_elem - 1) = xHat.tail(zHat.n_elem - 1) + 0.25 * (xHat.head(xHat.n_elem - 1) - xOpt.head(xOpt.n_elem - 1));

      const arma::Col<double>& z = 100.0 * (delta_ % (zHat - xOpt) + xOpt);

      return 0.01 * (418.9828872724339 - arma::mean(z % arma::sin(arma::sqrt(arma::abs(z))))) + 100.0 * getPenality(z / 100.0);
    }

    inline std::string SchwefelFunction::to_string() const noexcept {
      return "SchwefelFunction";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::SchwefelFunction);
#endif
