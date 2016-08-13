#include "mantellaMex.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 2) {
    mexErrMsgTxt("The number of input variables must be 2");
  } else if (nlhs > 1) {
    mexErrMsgTxt("The maximal number of output variables must be 1.");
  }

  try {
    const arma::uword numberOfElements = getIntegerScalar(prhs[0]);
    const arma::uword cycleSize = getIntegerScalar(prhs[1]);
    
    arma::mat multicombinations(cycleSize, mant::nchoosek(numberOfElements + cycleSize - 1, cycleSize));
    arma::uword n = 0;
    for (const auto& combination : mant::multicombinations(numberOfElements, cycleSize)) {
      multicombinations.col(n++) = arma::conv_to<arma::vec>::from(combination);
    }
    
    plhs[0] = getMxArray(multicombinations);
  } catch (const std::exception& exception) {
    std::cout << exception.what() << std::endl;
  }
}
