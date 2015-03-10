namespace mant {
  namespace bbob2009 {
    class RosenbrockFunctionRotated : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit RosenbrockFunctionRotated(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline std::string toString() const noexcept override;

      protected:
        const double max_;

        arma::Mat<double> rotationR_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("rotationR", rotationR_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<RosenbrockFunctionRotated>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
        }
#endif
    };

    //
    // Implementation
    //

    inline RosenbrockFunctionRotated::RosenbrockFunctionRotated(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        max_(std::max(1.0, std::sqrt(static_cast<double>(numberOfDimensions_)) / 8.0)) {
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void RosenbrockFunctionRotated::setRotationR(
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

    inline double RosenbrockFunctionRotated::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = max_ * rotationR_ * parameter + 0.5;

      return 100.0 * arma::accu(arma::square(arma::square(z.head(z.n_elem - 1)) - z.tail(z.n_elem - 1))) + arma::accu(arma::square(z.head(z.n_elem - 1) - 1.0));
    }

    inline std::string RosenbrockFunctionRotated::toString() const noexcept {
      return "rosenbrock-function-rotated";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::RosenbrockFunctionRotated);
#endif
