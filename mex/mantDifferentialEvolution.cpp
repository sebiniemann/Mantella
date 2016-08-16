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
    mant::DifferentialEvolution optimisationAlgorithm;

    mxArray* serialisedOptimisationAlgorithm = getMxArray(static_cast<mant::OptimisationAlgorithm>(optimisationAlgorithm));
    mxSetField(serialisedOptimisationAlgorithm, 0, "functionHandle", mxCreateString("Differential evolution"));
    mxAddField(serialisedOptimisationAlgorithm, "scalingFactor");
    mxSetField(serialisedOptimisationAlgorithm, 0, "scalingFactor", getMxArray(optimisationAlgorithm.getScalingFactor()));

    plhs[0] = serialisedOptimisationAlgorithm;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
