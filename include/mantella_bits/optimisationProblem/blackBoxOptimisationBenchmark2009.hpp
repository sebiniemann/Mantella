namespace mant {
  namespace bbob2009 {
    class BlackBoxOptimisationBenchmark2009 : public OptimisationProblem<double> {
      public:
        inline explicit BlackBoxOptimisationBenchmark2009(
            const unsigned int& numberOfDimensions) noexcept;

        inline virtual void setLocalTranslation(
            const arma::Col<double>& localTranslation);

        inline virtual void setReflection(
            const bool reflection);

        inline virtual void setRotationR(
            const arma::Mat<double>& rotationR);

        inline virtual void setRotationQ(
            const arma::Mat<double>& rotationQ);

        inline virtual void setScaling(
            const arma::Col<double>& scaling);

        inline virtual void setLocalOptima(
            const arma::Mat<double>& localOptima);

        virtual ~BlackBoxOptimisationBenchmark2009() = default;

      protected:
        arma::Col<double> localTranslation_;
        bool reflection_;
        arma::Mat<double> rotationR_;
        arma::Mat<double> rotationQ_;
        arma::Col<double> scaling_;
        arma::Mat<double> localOptima_;

        inline arma::Col<double> getRandomScaling() const noexcept;

        inline arma::Mat<double> getRandomLocalOptima() const noexcept;

        inline arma::Col<double> getScalingTransformation(
            const double& condition) const noexcept;

        inline arma::Col<double> getScalingTransformation(
            const arma::Col<double>& condition) const noexcept;

        inline arma::Col<double> getAsymmetricTransformation(
            const double& beta, const arma::Col<double>& parameter) const noexcept;

        inline double getOscillationTransformation(
            const double& value) const noexcept;

        inline arma::Col<double> getOscillationTransformation(
            const arma::Col<double>& parameter) const noexcept;

        inline double getPenality(
            const arma::Col<double>& parameter) const noexcept;

#if defined(MANTELLA_USE_PARALLEL)
        friend class cereal::access;
        BlackBoxOptimisationBenchmark2009() = default;

        template <typename Archive>
        void serialize(
            Archive& archive) noexcept {
          archive(cereal::make_nvp("optimisationProblem", cereal::base_class<OptimisationProblem>(this)));
          archive(cereal::make_nvp("xOpt", xOpt_));
        }
#endif
    };

    //
    // Implementation
    //

    inline BlackBoxOptimisationBenchmark2009::BlackBoxOptimisationBenchmark2009(
        const unsigned int& numberOfDimensions) noexcept
      : OptimisationProblem(numberOfDimensions) {
      setLowerBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) - 5.0);
      setUpperBounds(arma::zeros<arma::Col<double>>(numberOfDimensions_) + 5.0);
      setObjectiveValueTranslation(std::min(1000.0, std::max(-1000.0, std::cauchy_distribution<double>(0.0, 100.0)(Rng::getGenerator()))));

      setAcceptableObjectiveValue(objectiveValueTranslation_ + 1.0e-8);

      arma::Col<double> xOpt = arma::floor(arma::randu<arma::Col<double>>(numberOfDimensions_) * 1.0e4) / 1.0e4 * 8.0 - 4.0;
      xOpt.elem(arma::find(xOpt == 0)).fill(-1.0e5);
      setXOpt(xOpt);

      setOne(arma::zeros<arma::Col<double>>(numberOfDimensions_) + (std::bernoulli_distribution(0.5)(Rng::getGenerator()) ? 1.0 : -1.0));
      setRotationR(getRandomRotationMatrix(numberOfDimensions_));
      setRotationQ(getRandomRotationMatrix(numberOfDimensions_));
      setDeltaC101(getRandomDeltaC101());
      setLocalOptimaY101(getRandomLocalOptimaY101());
      setDeltaC21(getRandomDeltaC21());
      setLocalOptimaY21(getRandomLocalOptimaY21());
    }

    inline void BlackBoxOptimisationBenchmark2009::setXOpt(
        const arma::Col<double> xOpt) {
      if (xOpt.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the x^opt variable (" + std::to_string(xOpt.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      xOpt_ = xOpt;
    }

    inline void BlackBoxOptimisationBenchmark2009::setOne(
        const arma::Col<double> one) {
      if (one.n_elem != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the one vector (" + std::to_string(one.n_elem) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      }

      one_ = one;
    }

    inline void BlackBoxOptimisationBenchmark2009::setRotationR(
        const arma::Mat<double> rotationR) {
      if (!rotationR.is_square()) {
        throw std::logic_error("The rotation matrix (" + std::to_string(rotationR.n_rows) + ", " + std::to_string(rotationR.n_cols) + ") must be square.");
      } else if (rotationR.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(rotationR.n_rows) + ", " + std::to_string(rotationR.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if(arma::any(arma::vectorise(arma::abs(rotationR.i() - rotationR.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(rotationR))))))) {
        throw std::logic_error("The rotation matrix must be orthonormal.");
      } else if(std::abs(std::abs(arma::det(rotationR)) - 1.0) > 1.0e-12) {
        throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(rotationR)) + ") must be either 1 or -1.");
      }

      rotationR_ = rotationR;
    }

    inline void BlackBoxOptimisationBenchmark2009::setRotationQ(
        const arma::Mat<double> rotationQ) {
      if (!rotationQ.is_square()) {
        throw std::logic_error("The rotation matrix (" + std::to_string(rotationQ.n_rows) + ", " + std::to_string(rotationQ.n_cols) + ") must be square.");
      } else if (rotationQ.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of the parameter rotation maxtrix (" + std::to_string(rotationQ.n_rows) + ", " + std::to_string(rotationQ.n_cols) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if(arma::any(arma::vectorise(arma::abs(rotationQ.i() - rotationQ.t()) > 1.0e-12 * std::max(1.0, std::abs(arma::median(arma::vectorise(rotationQ))))))) {
        throw std::logic_error("The rotation matrix must be orthonormal.");
      } else if(std::abs(std::abs(arma::det(rotationQ)) - 1.0) > 1.0e-12) {
        throw std::logic_error("The rotation matrix's determinant (" + std::to_string(arma::det(rotationQ)) + ") must be either 1 or -1.");
      }

      rotationQ_ = rotationQ;
    }

    inline void BlackBoxOptimisationBenchmark2009::setDeltaC101(
        const arma::Mat<double> deltaC101) {
      if (deltaC101.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of each delta (" + std::to_string(deltaC101.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if (deltaC101.n_cols != 101) {
        throw std::logic_error("The number of deltas (" + std::to_string(deltaC101.n_cols) + ") must be 101.");
      }

      deltaC101_ = deltaC101;
    }

    inline void BlackBoxOptimisationBenchmark2009::setLocalOptimaY101(
        const arma::Mat<double> localOptimaY101) {
    if (localOptimaY101.n_rows != numberOfDimensions_) {
      throw std::logic_error("The number of dimensions of each local optimum (" + std::to_string(localOptimaY101.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
    } else if (localOptimaY101.n_cols != 101) {
      throw std::logic_error("The number of local optima (" + std::to_string(localOptimaY101.n_cols) + ") must be 101.");
    }

      localOptimaY101_ = localOptimaY101;
    }

    inline void BlackBoxOptimisationBenchmark2009::setDeltaC21(
        const arma::Mat<double> deltaC21) {
      if (deltaC21.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of each delta (" + std::to_string(deltaC21.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if (deltaC21.n_cols != 21) {
        throw std::logic_error("The number of deltas (" + std::to_string(deltaC21.n_cols) + ") must be 21.");
      }

      deltaC21_ = deltaC21;
    }

    inline void BlackBoxOptimisationBenchmark2009::setLocalOptimaY21(
        const arma::Mat<double> localOptimaY21) {
      if (localOptimaY21.n_rows != numberOfDimensions_) {
        throw std::logic_error("The number of dimensions of each local optimum (" + std::to_string(localOptimaY21.n_rows) + ") must match the number of dimensions of the optimisation problem (" + std::to_string(numberOfDimensions_) + ").");
      } else if (localOptimaY21.n_cols != 21) {
        throw std::logic_error("The number of local optima (" + std::to_string(localOptimaY21.n_cols) + ") must be 21.");
      }

      localOptimaY21_ = localOptimaY21;
    }

    inline arma::Mat<double> BlackBoxOptimisationBenchmark2009::getRandomDeltaC101() const noexcept {
      arma::Mat<double> deltaC101(numberOfDimensions_, 101);
      deltaC101.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

      std::uniform_int_distribution<unsigned int> uniformIntDistribution(0, 99);
      for (std::size_t n = 1; n < deltaC101.n_cols; ++n) {
        deltaC101.col(n) = getScaling(sqrt(1000.0)) / pow(pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Rng::getGenerator())) / 99.0), 0.25);
      }

      return deltaC101;
    }

    inline arma::Mat<double> BlackBoxOptimisationBenchmark2009::getRandomDeltaC21() const noexcept {
      arma::Mat<double> deltaC21(numberOfDimensions_, 21);
      deltaC21.col(0) = getScaling(std::sqrt(1000.0)) / std::pow(1000.0, 0.25);

      std::uniform_int_distribution<unsigned int> uniformIntDistribution(0, 19);
      for (std::size_t n = 1; n < deltaC21.n_cols; ++n) {
        deltaC21.col(n) = getScaling(sqrt(1000.0)) / std::pow(std::pow(1000.0, 2.0 * static_cast<double>(uniformIntDistribution(Rng::getGenerator())) / 19.0), 0.25);
      }

      return deltaC21;
    }

    inline arma::Mat<double> BlackBoxOptimisationBenchmark2009::getRandomLocalOptimaY101() const noexcept {
      arma::Mat<double> localOptimaY101 = arma::randu<arma::Mat<double>>(numberOfDimensions_, 101) * 8.0 - 4.0;
      localOptimaY101.col(0) = 0.8 * localOptimaY101.col(0);

      return localOptimaY101;
    }

    inline arma::Mat<double> BlackBoxOptimisationBenchmark2009::getRandomLocalOptimaY21() const noexcept {
      arma::Mat<double> localOptimaY21 = arma::randu<arma::Mat<double>>(numberOfDimensions_, 21) * 9.8 - 4.9;
      localOptimaY21.col(0) = 0.8 * localOptimaY21.col(0);

      return localOptimaY21;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getScaling(
        const double& condition) const noexcept {
      arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (auto& scale : scaling) {
        scale = std::pow(condition, scale);
      }

      return scaling;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getScaling(
        const arma::Col<double>& condition) const noexcept {
      arma::Col<double> scaling = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < scaling.n_elem; ++n) {
        scaling(n) = std::pow(condition(n), scaling(n));
      }

      return scaling;
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getAsymmetricTransformation(
        const double& beta,
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> asymmetricTransformation(parameter.n_elem);
      const arma::Col<double>& spacing = arma::linspace<arma::Col<double>>(0.0, 1.0, numberOfDimensions_);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        const double& value = parameter(n);

        if (value > 0.0) {
          asymmetricTransformation(n) = std::pow(value, 1 + beta * spacing(n) * std::sqrt(value));
        } else {
          asymmetricTransformation(n) = value;
        }
      }

      return asymmetricTransformation;
    }

    inline double BlackBoxOptimisationBenchmark2009::getOscillationTransformation(
        const double& value) const noexcept {
      if (value != 0.0) {
        double c1;
        double c2;
        if (value > 0.0) {
          c1 = 10.0;
          c2 = 7.9;
        } else {
          c1 = 5.5;
          c2 = 3.1;
        }

        const double& x = std::log(std::abs(value));
        return std::copysign(1.0, value) * std::exp(x + 0.049 * (std::sin(c1 * x) + std::sin(c2 * x)));
      } else {
        return 0.0;
      }
    }

    inline arma::Col<double> BlackBoxOptimisationBenchmark2009::getOscillationTransformation(
        const arma::Col<double>& parameter) const noexcept {
      arma::Col<double> oscillate(parameter.n_elem);

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        oscillate(n) = getOscillationTransformation(parameter(n));
      }

      return oscillate;
    }

    inline double BlackBoxOptimisationBenchmark2009::getPenality(
        const arma::Col<double>& parameter) const noexcept {
      double penality = 0.0;

      for (std::size_t n = 0; n < parameter.n_elem; ++n) {
        penality += std::pow(std::max(0.0, std::abs(parameter(n)) - 5.0), 2.0);
      }

      return penality;
    }
  }
}
