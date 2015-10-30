#include "mantella_bits/propertyAnalysis/passivePropertyAnalysis.hpp"

// C++ standard library
#include <utility>

// Mantella
#include "mantella_bits/helper/assert.hpp"

namespace mant {
  PassivePropertyAnalysis::PassivePropertyAnalysis(
      const std::unordered_map<arma::Col<double>, double, Hash, IsEqual>& samples)
      : numberOfDimensions_(samples.cbegin()->first.n_elem),
        samples_(samples) {
    verify(samples_.size() > 1, "");
    verify(isDimensionallyConsistent(samples), "");
  }

  void PassivePropertyAnalysis::analyse() {
    analyseImplementation();
  }
}
