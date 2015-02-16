namespace mant {
  namespace bbob2009 {
    class LinearSlope : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit LinearSlope(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setOne(
            const arma::Col<double> one) override;

        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> xOpt_;
        arma::Col<double> scaling_;
        double partiallyObjectiveValue_;

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
          archive(cereal::make_nvp("xOpt", xOpt_));
          archive(cereal::make_nvp("scaling", scaling_));
          archive(cereal::make_nvp("partiallyObjectiveValue", partiallyObjectiveValue_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LinearSlope>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("one", construct->one_));
          archive(cereal::make_nvp("xOpt", construct->xOpt_));
          archive(cereal::make_nvp("scaling", construct->scaling_));
          archive(cereal::make_nvp("partiallyObjectiveValue", construct->partiallyObjectiveValue_));
        }
#endif
    };

    //
    // Implementation
    //

    inline LinearSlope::LinearSlope(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions) {
      setOne(one_);
    }

    inline void LinearSlope::setOne(const arma::Col<double> one) {
      if (one.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the one vector (" + std::to_string(one.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      one_ = one;
      xOpt_ = 5.0 * one_;
      scaling_ = arma::sign(one_) % getScaling(10.0);
      partiallyObjectiveValue_ = 5.0 * arma::accu(arma::abs(scaling_));
    }

    inline double LinearSlope::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> z = parameter;

      const arma::Col<unsigned int>& outOfBound = arma::find(xOpt_ % z >= 25.0);
      z.elem(outOfBound) = xOpt_.elem(outOfBound);

      return partiallyObjectiveValue_ - arma::dot(scaling_, z);
    }

    inline std::string LinearSlope::toString() const noexcept {
      return "LinearSlope";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::LinearSlope);
#endif
