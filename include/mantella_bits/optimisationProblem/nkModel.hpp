#pragma once

// Mantella
#include <mantella_bits/optimisationProblem.hpp>

namespace mant {
  class NKModel : public OptimisationProblem<unsigned int> {
    public:
      explicit NKModel(
          const unsigned int& numberOfNeighbours,
          const unsigned int& numberOfDimensions) ;

      // Copy constructors are not used in this library and deleted to avoid unintended/any usage.
      NKModel(const NKModel&) = delete;
      NKModel& operator=(const NKModel&) = delete;

    protected:
      inline double getObjectiveValueImplementation(
          const arma::Col<unsigned int>& parameter) const  override;
  };

  NKModel::NKModel(
      const unsigned int& numberOfNeighbours,
      const unsigned int& numberOfDimensions) 
    : OptimisationProblem(numberOfDimensions){

  }

  inline double NKModel::getObjectiveValueImplementation(
      const arma::Col<unsigned int>& parameter) const  {
    return arma::sum(parameter);
  }
}
