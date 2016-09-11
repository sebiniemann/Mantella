#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 4) {
    mexErrMsgTxt("The number of input variables must be 4.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    arma::mat circleCircleIntersections(2, 2);
    arma::uword n = 0;
    for (const auto& circleCircleIntersection : mant::circleCircleIntersections(getRealColumnVector(prhs[0]), getRealScalar(prhs[1]), getRealColumnVector(prhs[2]), getRealScalar(prhs[3]))) {
      circleCircleIntersections.col(n++) = circleCircleIntersection;
    }
    circleCircleIntersections.resize(2, n);

    plhs[0] = getMxArray(circleCircleIntersections);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
