#pragma once

// Mantella
#include <mantella_bits/optimisationAlgorithm/trajectoryBasedAlgorithm.hpp>
#include <mantella_bits/distanceFunction/euclideanDistance.hpp>

namespace mant {
  class HookeJeevesAlgorithm : public TrajectoryBasedAlgorithm<double, EuclideanDistance> {
    public:
      explicit HookeJeevesAlgorithm(
          const std::shared_ptr<OptimisationProblem<double>> optimisationProblem) noexcept;

      HookeJeevesAlgorithm(const HookeJeevesAlgorithm&) = delete;
      HookeJeevesAlgorithm& operator=(const HookeJeevesAlgorithm&) = delete;

      void setInitialStepSize(
          const arma::Col<double>& initialStepSize);

      void setStepSizeDecrease(
          const arma::Col<double>& stepSizeDecrease);

      std::string to_string() const noexcept override;

    protected:
      arma::Col<double> initialStepSize_;
      arma::Col<double> stepSizeDecrease_;

      void optimiseImplementation() noexcept override;
  };
}
