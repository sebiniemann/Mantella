#pragma once

#include <memory>

#include <cereal/types/memory.hpp>
#include <cereal/access.hpp>

#include <hop>

#include <plot/plot.hpp>

class Plot2DFitnessFunction : public Plot {
  public:
    Plot2DFitnessFunction(const std::shared_ptr<hop::OptimisationProblem> optimisationProblem);

  protected:
    std::shared_ptr<hop::OptimisationProblem> _optimisationProblem = std::shared_ptr<hop::OptimisationProblem>(new hop::AttractiveSectorFunction(2));

    void runImpelementation();

  private:
    friend class cereal::access;

    Plot2DFitnessFunction() = default;

    template<class T>
    void serialize(T& archive) {
      archive(CEREAL_NVP(_optimisationProblem));
    }
};
