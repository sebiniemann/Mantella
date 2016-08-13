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
    plhs[0] = getMxArray(mant::timeOfFlight(getRealScalar(prhs[0]), getRealVector(prhs[1]), getRealVector(prhs[2]), mxIsLogicalScalarTrue(prhs[3])));
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
