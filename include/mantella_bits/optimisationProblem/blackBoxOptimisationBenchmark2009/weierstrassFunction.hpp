namespace mant {
  namespace bbob2009 {
    class WeierstrassFunction : public BlackBoxOptimisationBenchmark2009 {
      public:
        inline explicit WeierstrassFunction(
            const unsigned int& numberOfDimensions) noexcept;

        inline void setLocalParameterTranslation(
            const arma::Col<double>& localParameterTranslation);

        inline void setRotationR(
            const arma::Mat<double>& rotationR);

        inline void setRotationQ(
            const arma::Mat<double>& rotationQ);
            
        inline std::string toString() const noexcept override;

      protected:
        arma::Col<double> localParameterTranslation_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;
        
        double predefinedObjectiveValue_;
        const arma::Col<double> scaling_;

        inline double getObjectiveValueImplementation(
            const arma::Col<double>& parameter) const noexcept override;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(this)));
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions_));
          archive(cereal::make_nvp("localParameterTranslation", localParameterTranslation_));
          archive(cereal::make_nvp("rotationR", rotationR_));
          archive(cereal::make_nvp("rotationQ", rotationQ_));
        }

        template <typename Archive>
        static void load_and_construct(
            Archive& archive,
            cereal::construct<WeierstrassFunction>& construct) noexcept {
          unsigned int numberOfDimensions;
          archive(cereal::make_nvp("numberOfDimensions", numberOfDimensions));
          construct(numberOfDimensions);

          archive(cereal::make_nvp("BlackBoxOptimisationBenchmark2009", cereal::base_class<BlackBoxOptimisationBenchmark2009>(construct.ptr())));
          archive(cereal::make_nvp("localParameterTranslation", construct->localParameterTranslation_));
          archive(cereal::make_nvp("rotationR", construct->rotationR_));
          archive(cereal::make_nvp("rotationQ", construct->rotationQ_));
        }
#endif
    };

    //
    // Implementation
    //

    inline WeierstrassFunction::WeierstrassFunction(
        const unsigned int& numberOfDimensions) noexcept
      : BlackBoxOptimisationBenchmark2009(numberOfDimensions),
        scaling_(getScaledTransformation(std::sqrt(0.01))) {
      predefinedObjectiveValue_ = 0.0;
      for (unsigned int k = 0; k < 12; ++k) {
        predefinedObjectiveValue_ += std::pow(0.5, k) * cos(arma::datum::pi * pow(3.0, k));
      }
      
      setLocalParameterTranslation(getRandomLocalParameterTranslation());
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
    }

    inline void WeierstrassFunction::setLocalParameterTranslation(
        const arma::Col<double>& localParameterTranslation) {
      if (localParameterTranslation.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the local translation (" + std::to_string(localParameterTranslation.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      localParameterTranslation_ = localParameterTranslation;
    }

    inline void WeierstrassFunction::setRotationR(
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

    inline void WeierstrassFunction::setRotationQ(
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
    
    inline double WeierstrassFunction::getObjectiveValueImplementation(
        const arma::Col<double>& parameter) const noexcept {
      const arma::Col<double>& z = rotationR_ * (scaling_ % (rotationQ_ * getOscillatedTransformation(rotationR_ * (parameter - localParameterTranslation_))));

      double sum = 0.0;
      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        for (unsigned int k = 0; k < 12; ++k) {
          sum += std::pow(0.5, k) * std::cos(2.0 * arma::datum::pi * std::pow(3.0, k) * (z(n) + 0.5));
        }
      }

      return 10 * (std::pow(sum / static_cast<double>(numberOfDimensions_) - predefinedObjectiveValue_, 3) + getPenality(parameter) / static_cast<double>(numberOfDimensions_));
    }

    inline std::string WeierstrassFunction::toString() const noexcept {
      return "weierstrass-function";
    }
  }
}

#if defined(MANTELLA_USE_PARALLEL)
// CEREAL_REGISTER_TYPE(mant::bbob2009::WeierstrassFunction);
#endif
