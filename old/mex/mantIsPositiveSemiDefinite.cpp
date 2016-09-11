#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 1) {
    mexErrMsgTxt("The number of input variables must be 1.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    plhs[0] = mxCreateLogicalScalar(mant::isPositiveSemiDefinite(getRealMatrix(prhs[0])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
