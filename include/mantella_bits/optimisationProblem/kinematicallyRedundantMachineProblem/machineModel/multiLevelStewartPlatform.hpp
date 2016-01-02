#pragma once

// C++ standard library
#include <vector>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel.hpp"
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel/parallelKinematicMachine6PUPS.hpp"

namespace mant {
  namespace krmp {
    class MultiLevelStewartPlatform : public MachineModel {
     public:
      // Another name for a single Stewart platform is 6PUPS parallel kinematic machine.
      const std::vector<ParallelKinematicMachine6PUPS> platformLevels_;
      const arma::uword numberOfPlatformLevels_;

      explicit MultiLevelStewartPlatform();

      explicit MultiLevelStewartPlatform(
          const std::vector<ParallelKinematicMachine6PUPS>& platformLevels);
    };
  }
}
