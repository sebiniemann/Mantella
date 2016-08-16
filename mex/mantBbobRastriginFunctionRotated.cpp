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
    bbob::RastriginFunctionRotated optimisationProblem(getIntegerScalar(prhs[0]));

    mxArray* serialisedOptimisationProblem = getMxArray(static_cast<mant::BlackBoxOptimisationBenchmark>(optimisationProblem));
    mxSetField(serialisedOptimisationProblem, 0, "functionHandle", mxCreateString("BBOB Rastrigin Function, rotated (f15)"));
    mxAddField(serialisedOptimisationProblem, "rotationQ");
    mxSetField(serialisedOptimisationProblem, 0, "rotationQ", getMxArray(optimisationProblem.rotationQ_));
    mxAddField(serialisedOptimisationProblem, "rotationR");
    mxSetField(serialisedOptimisationProblem, 0, "rotationR", getMxArray(optimisationProblem.rotationR_));

    plhs[0] = serialisedOptimisationProblem;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
