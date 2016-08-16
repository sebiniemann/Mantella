#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs < 2 || nrhs > 3) {
    mexErrMsgTxt("The number of input variables must be 2 or 3.");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    std::vector<arma::uvec> partition;
    switch (nrhs) {
      case 2: {
        partition = mant::additiveSeparability(*getOptimisationProblem(prhs[0]), getIntegerScalar(prhs[1]));
      } break;
      case 3:
      default: {
        partition = mant::additiveSeparability(*getOptimisationProblem(prhs[0]), getIntegerScalar(prhs[1]), getRealScalar(prhs[2]));
      } break;
    }

    mxArray* mxPartition = mxCreateCellMatrix(static_cast<mwSize>(partition.size()), 1);
    if (mxPartition == nullptr) {
      mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
    } else {
      arma::uword n = 0;
      for (const auto& part : partition) {
        mxSetCell(mxPartition, static_cast<mwIndex>(n++), getMxArray(arma::conv_to<arma::vec>::from(part)));
      }
    }
    plhs[0] = mxPartition;
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
