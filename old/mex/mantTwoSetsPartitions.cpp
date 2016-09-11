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
    const arma::uword numberOfElements = getIntegerScalar(prhs[0]);
    const arma::uword numberOfTwoSetPartitions = mant::secondStirlingNumber(numberOfElements, 2);

    mxArray* twoSetsPartitions = mxCreateCellMatrix(static_cast<mwSize>(numberOfTwoSetPartitions), 2);
    if (twoSetsPartitions == nullptr) {
      mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
    } else {
      arma::uword n = 0;
      for (const auto& twoSetsPartition : mant::twoSetsPartitions(numberOfElements)) {
        mxSetCell(twoSetsPartitions, static_cast<mwIndex>(n), getMxArray(arma::conv_to<arma::vec>::from(twoSetsPartition.first)));
        mxSetCell(twoSetsPartitions, static_cast<mwIndex>(n + numberOfTwoSetPartitions), getMxArray(arma::conv_to<arma::vec>::from(twoSetsPartition.second)));
        ++n;
      }
    }
    plhs[0] = twoSetsPartitions;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
