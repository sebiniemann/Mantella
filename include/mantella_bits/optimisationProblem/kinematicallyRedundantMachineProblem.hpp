#pragma once

// C++ standard library
#include <memory>

// Mantella
#include "mantella_bits/optimisationProblem.hpp"
namespace mant {
  namespace krmp {
    class MachineModel;
  }
}

namespace mant {
  namespace krmp {
    class KinematicallyRedundantMachineProblem : public OptimisationProblem {
     public:
      KinematicallyRedundantMachineProblem(
          std::shared_ptr<MachineModel> machineModel);

     protected:
      std::shared_ptr<MachineModel> machineModel_;
    };
  }
}
