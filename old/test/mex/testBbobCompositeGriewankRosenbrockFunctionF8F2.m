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
    mxArray* serialisedOptimisationProblem = getMxArray(static_cast<mant::BlackBoxOptimisationBenchmark>(mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(getIntegerScalar(prhs[0]))));
    mxSetField(serialisedOptimisationProblem, 0, "functionHandle", mxCreateString("BBOB Composite Griewank Rosenbrock Function F8F2 (f19)"));
    plhs[0] = serialisedOptimisationProblem;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
