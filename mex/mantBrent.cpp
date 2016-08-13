#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 4) {
    mexErrMsgTxt("The number of input variables must be 4");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }
  
  try {
    if (!mxIsFunctionHandle(prhs[0])) {
      mexErrMsgTxt("The first parameter must be a function handle.");
    }
    
    mxArray* functionHandle = mxDuplicateArray(prhs[0]);
    auto objectiveFunction = [functionHandle](
          const double parameter_) {
        mxArray* functionInput[2] = {functionHandle, getMxArray(parameter_)};
        mxArray* functionOutput[1] = {mxCreateDoubleMatrix(1, 1, mxREAL)};
        mexCallMATLAB(1, functionOutput, 2, functionInput, "feval");
        return getRealScalar(functionOutput[0]);
      };
    
    plhs[0] = getMxArray(mant::brent(objectiveFunction, getRealScalar(prhs[1]), getRealScalar(prhs[2]), getIntegerScalar(prhs[3])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
