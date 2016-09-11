#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs < 2 || nrhs > 3) {
    mexErrMsgTxt("The number of input variables must be 2 or 3.");
  } else if (nlhs > 6) {
    mexErrMsgTxt("The maximal number of output variables must be 6.");
  }

  try {
    std::shared_ptr<mant::OptimisationAlgorithm> optimisationAlgorithm = getOptimisationAlgorithm(prhs[0]);
    std::shared_ptr<mant::OptimisationProblem> optimisationProblem = getOptimisationProblem(prhs[1]);

    switch (nrhs) {
      case 2: {
        optimisationAlgorithm->optimise(*optimisationProblem, arma::randu<arma::mat>(optimisationProblem->numberOfDimensions_, optimisationAlgorithm->getPopulationSize()));
      } break;
      case 3:
      default: {
        optimisationAlgorithm->optimise(*optimisationProblem, getRealMatrix(prhs[2]));
      } break;
    }

    plhs[0] = getMxArray(optimisationAlgorithm->getBestFoundParameter());
    if (nlhs > 1) {
      plhs[1] = getMxArray(optimisationAlgorithm->getBestFoundObjectiveValue());
    }
    if (nlhs > 2) {
      plhs[2] = mxCreateLogicalScalar(optimisationAlgorithm->isFinished());
    }
    if (nlhs > 3) {
      plhs[3] = mxCreateLogicalScalar(optimisationAlgorithm->isTerminated());
    }
    if (nlhs > 4) {
      plhs[4] = getMxArray(optimisationAlgorithm->getUsedNumberOfIterations());
    }
    if (nlhs > 5) {
      plhs[5] = getMxArray(static_cast<arma::uword>(optimisationAlgorithm->getUsedDuration().count()));
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
