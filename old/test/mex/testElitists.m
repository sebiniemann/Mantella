#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 3) {
    mexErrMsgTxt("The number of input variables must be 3.");
  } else if (nlhs > 2) {
    mexErrMsgTxt("The number of output variables must be 0, 1 or 2.");
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

      auto elitists = mant::elitists(samples, getIntegerScalar(prhs[2]));

      arma::mat elitistParameters(parameters.n_rows, elitists.size());
      arma::rowvec elitistObjectiveValues(elitists.size());
      arma::uword n = 0;
      for (const auto& elitist : elitists) {
        elitistParameters.col(n) = elitist.first;
        elitistObjectiveValues(n) = elitist.second;
        ++n;
      }

      plhs[0] = getMxArray(elitistParameters);
      if (nlhs > 1) {
        plhs[0] = getMxArray(elitistObjectiveValues);
      }
    }
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
