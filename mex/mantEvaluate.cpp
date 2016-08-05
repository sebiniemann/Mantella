#include "mexHelper.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  std::cout << "test stream" << std::endl;

  if (nrhs != 2) {
    mexErrMsgIdAndTxt("mantBbobSphereFunction", "The optimisation problem and parameter must be provided.");
  }

  std::shared_ptr<mant::OptimisationProblem> optimisationProblem = deserialise(prhs[0]);

  plhs[0] = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
  mxGetPr(plhs[0])[0] = optimisationProblem->getObjectiveValue(arma::vec(static_cast<double*>(mxGetData(prhs[1])), 2));
}