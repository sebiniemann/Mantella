#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/lipschitzContinuityAnalysis.hpp>

// C++ standard library
#include <cassert>
#include <algorithm>
#include <cmath>

// Armadillo
#include <armadillo>

namespace mant {
  void LipschitzContinuityAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);

    lipschitzConstant_ = 0.0;
    for (auto n = samples_.cbegin(); n != samples_.cend();) {
      const arma::Col<double>& parameter = n->first;
      const double objectiveValue = n->second;
      for (auto k = ++n; k != samples_.cend(); ++k) {
        lipschitzConstant_ = std::max(lipschitzConstant_, std::abs(k->second - objectiveValue) / arma::norm(k->first - parameter));
      }
    }
  }

  double LipschitzContinuityAnalysis::getLipschitzConstant() const {
    return lipschitzConstant_;
  }

  std::string LipschitzContinuityAnalysis::toString() const {
    return "lipschitz_continuity_analysis";
  }
}
