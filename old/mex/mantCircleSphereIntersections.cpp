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
    arma::mat circleSphereIntersections(3, 2);
    arma::uword n = 0;
    for (const auto& circleSphereIntersection : mant::circleSphereIntersections(getRealColumnVector(prhs[0]), getRealScalar(prhs[1]), getRealColumnVector(prhs[2]), getRealColumnVector(prhs[3]), getRealScalar(prhs[4]))) {
      circleSphereIntersections.col(n++) = circleSphereIntersection;
    }
    circleSphereIntersections.resize(3, n);

    plhs[0] = getMxArray(circleSphereIntersections);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
