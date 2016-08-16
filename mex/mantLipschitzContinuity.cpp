#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 2) {
    mexErrMsgTxt("The number of input variables must be 2.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The number of output variables must be 0 or 1.");
  }

  try {
    const arma::mat& parameters = getRealMatrix(prhs[0]);
    const arma::rowvec& objectiveValues = getRealVector(prhs[1]).t();

    if (parameters.n_cols != objectiveValues.n_elem) {
      mexErrMsgTxt("The number of parameters must be equal to the number of objective values.");
    } else {
      std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual> samples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        samples.insert({parameters.col(n), objectiveValues(n)});
      }

      plhs[0] = getMxArray(mant::lipschitzContinuity(samples));
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
