#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs < 1 || nrhs > 4) {
    mexErrMsgTxt("The number of input variables must be 1, 2, 3 or 4.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    switch (nrhs) {
      case 1: {
        plhs[0] = getMxArray(mant::uniformRandomNumbers(getIntegerScalar(prhs[0])));
      } break;
      case 2: {
        plhs[0] = getMxArray(mant::uniformRandomNumbers(getIntegerScalar(prhs[0]), getIntegerScalar(prhs[1])));
      } break;
      case 3: {
        plhs[0] = getMxArray(mant::uniformRandomNumbers(getIntegerScalar(prhs[0]), std::uniform_real_distribution<double>(getRealScalar(prhs[1]), getRealScalar(prhs[2]))));
      } break;
      case 4:
      default: {
        plhs[0] = getMxArray(mant::uniformRandomNumbers(getIntegerScalar(prhs[0]), getIntegerScalar(prhs[1]), std::uniform_real_distribution<double>(getRealScalar(prhs[2]), getRealScalar(prhs[3]))));
      } break;
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
