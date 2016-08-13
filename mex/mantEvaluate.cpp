#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();
  
  std::cout << "test stream" << std::endl;
  if (nrhs != 2) {
    mexErrMsgTxt("mantEvaluate: The number of input variables must be 2");
  } else if (nlhs > 1) {
    mexErrMsgTxt("mantEvaluate: The maximal number of output variables must be 1.");
  }

  try {
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem = getOptimisationProblem(prhs[0]);
    plhs[0] = getMxArray(optimisationProblem->getObjectiveValue(getRealVector(prhs[1])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}