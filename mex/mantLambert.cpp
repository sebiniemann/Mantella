#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 5) {
    mexErrMsgTxt("The number of input variables must be 5.");
  } else if (nlhs > 2) {
    mexErrMsgTxt("The number of output variables must be 0, 1 or 2.");
  }

  try {
    auto velocities = mant::lambert(getRealVector(prhs[0]), getRealVector(prhs[1]), mxIsLogicalScalarTrue(prhs[2]), getIntegerScalar(prhs[3]), getRealScalar(prhs[4]));

    plhs[0] = getMxArray(velocities.first);
    if (nlhs > 1) {
      plhs[0] = getMxArray(velocities.second);
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
