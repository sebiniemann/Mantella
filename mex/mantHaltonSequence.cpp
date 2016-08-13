#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs < 2 || nrhs > 3) {
    mexErrMsgTxt("The number of input variables must be 2 or 3");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    switch (nrhs) {
      case 2: {
        plhs[0] = getMxArray(mant::haltonSequence(getIntegerVector(prhs[0]), getIntegerScalar(prhs[1])));
      }
      case 3:
      default: {
        plhs[0] = getMxArray(mant::haltonSequence(getIntegerVector(prhs[0]), getIntegerVector(prhs[1]), getIntegerScalar(prhs[2])));
      }
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
