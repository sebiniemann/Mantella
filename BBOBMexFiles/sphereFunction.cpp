#include "mex.h"
#include <mantella>

class MatLabFunction : public mant::bbob::SphereFunction {
 public:
  using mant::bbob::SphereFunction::SphereFunction;

  // Increases the visibility of the internal objective function, to bypass general modification, made by the parent class.
  using mant::bbob::SphereFunction::objectiveFunction_;
};


void mexFunction (int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]){
	// Convert MatLab structure to mantella
	arma::Col<double> parameter(nrhs);
	for(int i=0; i<nrhs; i++){
		parameter(i) = *mxGetPr(prhs[i]);
	}
	
	MatLabFunction optimisationProblem(nrhs);
  	double objectiveValue = optimisationProblem.objectiveFunction_(parameter);
  	
  	// Return objective value to MatLab 
    plhs[0] = mxCreateNumericMatrix(1,1, mxDOUBLE_CLASS, mxREAL);
    *mxGetPr(plhs[0]) = objectiveValue;

    return;
}
