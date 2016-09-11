#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 2) {
    mexErrMsgTxt("The number of input variables must be 2.");
  } else if (nlhs > 2) {
    mexErrMsgTxt("The number of output variables must be 0, 1 or 2.");
  }

  try {
    auto positionAndVelocity = mant::positionAndVelocityOnOrbit(getRealScalar(prhs[0]), getRealVector(prhs[1]));

    plhs[0] = getMxArray(positionAndVelocity.first);
    if (nlhs > 1) {
      plhs[0] = getMxArray(positionAndVelocity.second);
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
