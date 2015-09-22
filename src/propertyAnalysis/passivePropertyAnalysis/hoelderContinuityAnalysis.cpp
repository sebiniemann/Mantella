#include <mantella_bits/propertyAnalysis/passivePropertyAnalysis/hoelderContinuityAnalysis.hpp>

// C++ standard library
#include <cassert>
#include <algorithm>
#include <cmath>

// Armadillo
#include <armadillo>

namespace mant {
  void HoelderContinuityAnalysis::analyseImplementation() {
    assert(samples_.size() > 1);

    hoelderExponent_ = 1;
    for (auto n = samples_.cbegin(); n != samples_.cend();) {
      const arma::Col<double>& parameter = n->first;
      const double objectiveValue = n->second;
      for (auto k = ++n; k != samples_.cend(); ++k) {
        hoelderExponent_ = std::min(hoelderExponent_, std::log(std::abs(k->second - objectiveValue)) / std::log(arma::norm(k->first - parameter)));
      }
    }
  }

  double HoelderContinuityAnalysis::getHoelderExponent() const {
    return hoelderExponent_;
  }

  std::string HoelderContinuityAnalysis::toString() const {
    return "hoelder_continuity_analysis";
  }
}
