#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 2) {
    mexErrMsgTxt("The number of input variables must be 2.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The number of output variables must be 0 or 1.");
  }

  try {
    plhs[0] = getMxArray(mant::fitnessDistanceCorrelation(getRealMatrix(prhs[0]), getRealRowVector(prhs[1])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
