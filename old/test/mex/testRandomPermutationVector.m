#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs < 1 || nrhs > 2) {
    mexErrMsgTxt("The number of input variables must be 1 or 2.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    switch (nrhs) {
      case 1: {
        plhs[0] = getMxArray(mant::randomPermutationVector(getIntegerScalar(prhs[0])));
      } break;
      case 2:
      default: {
        plhs[0] = getMxArray(mant::randomPermutationVector(getIntegerScalar(prhs[0]), getIntegerScalar(prhs[1])));
      } break;
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
