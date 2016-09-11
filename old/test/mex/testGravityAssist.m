#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 5) {
    mexErrMsgTxt("The number of input variables must be 5.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    plhs[0] = getMxArray(mant::gravityAssist(getRealVector(prhs[0]), getRealVector(prhs[1]), getRealVector(prhs[2]), getRealScalar(prhs[3]), getRealScalar(prhs[4])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
