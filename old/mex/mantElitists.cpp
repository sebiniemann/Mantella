#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 3) {
    mexErrMsgTxt("The number of input variables must be 3.");
  } else if (nlhs > 2) {
    mexErrMsgTxt("The number of output variables must be 0, 1 or 2.");
  }

  try {
    auto elitists = mant::elitists(getRealMatrix(prhs[0]), getRealRowVector(prhs[1]), getIntegerScalar(prhs[2]));
    plhs[0] = getMxArray(elitists.first);
    if (nlhs > 1) {
      plhs[0] = getMxArray(elitists.second);
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
