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
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    const arma::mat& parameters = getRealMatrix(prhs[0]);
    const arma::rowvec& objectiveValues = getRealVector(prhs[1]).t();

    if (parameters.n_cols != objectiveValues.n_elem) {
      plhs[0] = mxCreateLogicalScalar(false);
    } else {
      std::unordered_map<arma::vec, double, mant::Hash, mant::IsEqual> samples;
      for (arma::uword n = 0; n < parameters.n_cols; ++n) {
        samples.insert({parameters.col(n), objectiveValues(n)});
      }

      plhs[0] = mxCreateLogicalScalar(mant::isDimensionallyConsistent(samples));
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
