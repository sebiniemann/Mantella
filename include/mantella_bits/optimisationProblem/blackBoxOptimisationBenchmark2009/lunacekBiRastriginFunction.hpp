namespace mant {
  namespace bbob2009 {
    class LunacekBiRastriginFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit LunacekBiRastriginFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setParameterReflection(
            const bool Parameterreflection) noexcept;

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline std::string toString() const noexcept override;

      protected:
        const arma::Col<double> scaling_;
        const double s_ = 1.0 - 0.5 / (std::sqrt(static_cast<double>(numberOfDimensions_) + 20.0) - 4.1);
        const double mu1_ = -std::sqrt(5.25 / s_);

        arma::Col<double> Parameterreflection_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("Parameterreflection", Parameterreflection_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<LunacekBiRastriginFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("Parameterreflection", construct->Parameterreflection_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline LunacekBiRastriginFunction::LunacekBiRastriginFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(std::sqrt(10.0))) {
      setParameterReflection(std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? true : false);
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void LunacekBiRastriginFunction::setParameterReflection(
        const bool Parameterreflection) noexcept {
      if (Parameterreflection) {
        Parameterreflection_ = -arma::ones<arma::Col<double>>(numberOfDimensions_);
      } else {
        Parameterreflection_ = arma::ones<arma::Col<double>>(numberOfDimensions_);
      }
    }

    inline void LunacekBiRastriginFunction::setRotationR(
        const arma::Mat<double>& rotationR) {
      if (!rotationR.is_square()) {
        throw std::logic_error("The rotation matrix's shape (" + std::to_string(rotationR.n_rows) + ", " + std::to_string(rotationR.n_cols) + ") must be square.");
      } else if (rotationR.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(rotationR.n_rows) + ", " + std::to_string(rotationR.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if(arma::any(arma::vectorise(arma::abs(rotationR.i() - rotationR.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(rotationR))))))) {
        throw std::logic_error("The rotation matrix must be orthonormal.");
      } else if(std::abs(std::abs(arma::det(rotationR)) - 1.0) > 1.0e-12) {
        throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(rotationR)) + ") must be either 1 or -1.");
      }

      rotationR_ = rotationR;
    }

    inline void LunacekBiRastriginFunction::setRotationQ(
        const arma::Mat<double>& rotationQ) {
      if (!rotationQ.is_square()) {
        throw std::logic_error("The rotation matrix's shape (" + std::to_string(rotationQ.n_rows) + ", " + std::to_string(rotationQ.n_cols) + ") must be square.");
      } else if (rotationQ.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(rotationQ.n_rows) + ", " + std::to_string(rotationQ.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if(arma::any(arma::vectorise(arma::abs(rotationQ.i() - rotationQ.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(rotationQ))))))) {
        throw std::logic_error("The rotation matrix must be orthonormal.");
      } else if(std::abs(std::abs(arma::det(rotationQ)) - 1.0) > 1.0e-12) {
        throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(rotationQ)) + ") must be either 1 or -1.");
      }

      rotationQ_ = rotationQ;
    }

    inline double LunacekBiRastriginFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& xHat = 2.0 * arma::sign(Parameterreflection_) % parameter;
      const arma::Col<double>& z = rotationQ_ * (scaling_ % (rotationR_ * (xHat - 2.5)));

      return std::min(arma::accu(arma::square(xHat - 2.5)), static_cast<double>(numberOfDimensions_) + s_ * arma::accu(arma::square(xHat - mu1_))) + 10.0 * (static_cast<double>(numberOfDimensions_) - arma::accu(arma::cos(2.0 * arma::datum::pi * z))) + 10000.0 * getPenality(parameter);
    }

    inline std::string LunacekBiRastriginFunction::toString() const noexcept {
      return "lunacek-bi-rastrigin-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::LunacekBiRastriginFunction);
#endif
