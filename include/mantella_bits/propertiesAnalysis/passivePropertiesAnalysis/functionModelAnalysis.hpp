#pragma once

// Mantella
#include <mantella_bits/propertiesAnalysis/passivePropertiesAnalysis.hpp>

namespace mant {
  template <typename ParameterType, class DistanceFunction>
  class FunctionModelAnalysis : public PassivePropertiesAnalysis<ParameterType, DistanceFunction> {
    public:
      using PassivePropertiesAnalysis<ParameterType, DistanceFunction>::PassivePropertiesAnalysis;

      arma::Col<double> getResiduals() const ;

    protected:
      arma::Col<double> residuals_;
  };

  template <typename ParameterType, class DistanceFunction>
  arma::Col<double> FunctionModelAnalysis<ParameterType, DistanceFunction>::getResiduals() const  {
    return residuals_;
  }
}
