#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem.hpp"

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem/machineModel.hpp"

namespace mant {
  namespace krmp {
    KinematicallyRedundantMachineProblem::KinematicallyRedundantMachineProblem(
        std::shared_ptr<MachineModel> machineModel)
        : OptimisationProblem(machineModel->numberOfRedundantJoints_),
          machineModel_(machineModel) {
    }
  }
}
