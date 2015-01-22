namespace mant {
  namespace bbob2009 {
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit WeierstrassFunction(
            const unsigned int& numberOfDimensions) noexcept;

        WeierstrassFunction(const WeierstrassFunction&) = delete;
        WeierstrassFunction& operator=(const WeierstrassFunction&) = delete;

        inline std::string to_string() const noexcept override;

      protected:
        double f0_;
        const arma::Col<double> delta_ = getScaling(std::sqrt(0.01));

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
        friend class cereal::access;

        // TODO Add a default constructor to calculate f0 and remove it from cereal

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("translation", translation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
          archive(cereal::make_nvp("f0", f0_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<WeierstrassFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("translation", construct->translation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
          archive(cereal::make_nvp("f0", construct->f0_));
        }
#endif
    };

    inline WeierstrassFunction::WeierstrassFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      f0_ = 0.0;
      for (unsigned int k = 0; k < 12; ++k) {
        f0_ += std::pow(0.5, k) * cos(2.0 * arma::datum::pi * pow(3.0, k) * 0.5);
      }
    }

    inline double WeierstrassFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = rotationR_ * (delta_ % (rotationQ_ * getOscillationTransformation(rotationR_ * (parameter - translation_))));

      double sum = 0.0;
      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        for (unsigned int k = 0; k < 12; ++k) {
          sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z.at(n) + 0.5));
        }
      }

      return 10 * (std::pow(sum / static_cast<double>(numberOfDimensions_) - f0_, 3) + getPenality(parameter) / static_cast<double>(numberOfDimensions_));
    }

    inline std::string WeierstrassFunction::to_string() const noexcept {
      return "WeierstrassFunction";
    }
  }
}

#if defined(MANTELLA_BUILD_PARALLEL_VARIANTS)
// CEREAL_REGISTER_TYPE(mant::bbob2009::WeierstrassFunction);
#endif
