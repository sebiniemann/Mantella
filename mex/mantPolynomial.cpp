#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 2) {
    mexErrMsgTxt("The number of input variables must be 2");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    plhs[0] = getMxArray(mant::polynomial(getRealVector(prhs[0]), getIntegerScalar(prhs[1])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
