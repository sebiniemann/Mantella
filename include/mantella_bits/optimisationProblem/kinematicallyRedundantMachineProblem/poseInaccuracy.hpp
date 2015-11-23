#pragma once

// C++ standard library
#include <string>

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/optimisationProblem/kinematicallyRedundantMachineProblem.hpp"

namespace mant {
  namespace krmp {
    class EndEffectorPoseError : public KinematicallyRedundantMachineProblem {
    };
  }
}
