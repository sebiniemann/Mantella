#pragma once

#include <hop_bits/optimisationAlgorithm.hpp>

namespace hop {
  class CovarianceMatrixAdaptationEvolutionStrategy : public OptimisationAlgorithm {
    public:
      //TODO: which input parameters should be added to the constructor?
      explicit CovarianceMatrixAdaptationEvolutionStrategy(const std::shared_ptr<OptimisationProblem> optimisationProblem);

      CovarianceMatrixAdaptationEvolutionStrategy(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;
      CovarianceMatrixAdaptationEvolutionStrategy& operator=(const CovarianceMatrixAdaptationEvolutionStrategy&) = delete;
      
      std::string to_string() const override;

    protected:
      void optimiseImplementation() override;
      //input parameters
      int numberOfDimensions_; //N
      arma::Col<double> objectiveValues_; //xmean
	  double sigma_;
      double stopValue_; //stopfitness
	  
      //selection parameters
      int lambda_;
	  double mu_;
      arma::Col<double> weights_;
      double mueff_;
	  
      //adaptation parameters
	  double cc_;
	  double cs_;
	  double c1_;
	  double cmu_;
	  double damps_;
	  
      //dynamic parameters and constants
	  arma::Col<double> pc_;
	  arma::Col<double> ps_;
	  arma::Mat<double> B_;
      arma::Mat<double> D_;
	  arma::Mat<double> C_;
	  arma::Mat<double> invsqrtC_;
	  double eigeneval_;
	  double chiN_;
	  
      //function to use
      double frosenbrock(arma::Col<double> x);
  };
}
