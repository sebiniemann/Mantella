#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 0) {
    mexErrMsgTxt("The number of input variables must be 0.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    mxArray* serialisedOptimisationAlgorithm = getMxArray(static_cast<mant::OptimisationAlgorithm>(mant::RandomSearch()));
    mxSetField(serialisedOptimisationAlgorithm, 0, "functionHandle", mxCreateString("Random search"));
    plhs[0] = serialisedOptimisationAlgorithm;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
