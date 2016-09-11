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
    mant::HookeJeevesAlgorithm optimisationAlgorithm;

    mxArray* serialisedOptimisationAlgorithm = getMxArray(static_cast<mant::OptimisationAlgorithm>(optimisationAlgorithm));
    mxSetField(serialisedOptimisationAlgorithm, 0, "functionHandle", mxCreateString("Hooke-Jeeves algorithm"));
    mxAddField(serialisedOptimisationAlgorithm, "initialStepSize");
    mxSetField(serialisedOptimisationAlgorithm, 0, "initialStepSize", getMxArray(optimisationAlgorithm.getInitialStepSize()));
    mxAddField(serialisedOptimisationAlgorithm, "stepSizeDecrease");
    mxSetField(serialisedOptimisationAlgorithm, 0, "stepSizeDecrease", getMxArray(optimisationAlgorithm.getStepSizeDecrease()));

    plhs[0] = serialisedOptimisationAlgorithm;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
