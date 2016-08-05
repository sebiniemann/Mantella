#include "mexHelper.hpp"

void mexFunction(
    const int nlhs,
    mxArray* plhs[],
    const int nrhs,
    const mxArray* prhs[]) {
  initialise();

  if (nrhs != 1) {
    mexErrMsgIdAndTxt("mantBbobSphereFunction", "The number of dimensions must be provided");
  }

  const arma::uword numberOfDimensions = mxGetNumberOfElements(prhs[0]);
  if (numberOfDimensions < 1) {
    mexErrMsgIdAndTxt("mantBbobSphereFunction", "The number of dimensions must be greater than 0.");
  }

  if (nlhs > 1) {
    mexErrMsgIdAndTxt("mantBbobSphereFunction", "The maximal number of output variables must be 1.");
  }

  class AttractiveSectorFunction : public mant::bbob::AttractiveSectorFunction {
   public:
    using mant::bbob::AttractiveSectorFunction::AttractiveSectorFunction;
    using mant::bbob::AttractiveSectorFunction::rotationQ_;
  };
  AttractiveSectorFunction optimisationProblem(mxGetNumberOfElements(prhs[0]));

  mxArray* serialisedOptimisationProblem = serialise(static_cast<mant::BlackBoxOptimisationBenchmark>(optimisationProblem));
  mxAddField(serialisedOptimisationProblem, "rotationQ");
  mxArray* fieldRotationQ = mxCreateNumericMatrix(optimisationProblem.numberOfDimensions_, optimisationProblem.numberOfDimensions_, mxDOUBLE_CLASS, mxREAL);
  std::copy(optimisationProblem.rotationQ_.begin(), optimisationProblem.rotationQ_.end(), mxGetPr(fieldRotationQ));
  mxSetField(serialisedOptimisationProblem, 0, "rotationQ", fieldRotationQ);

  plhs[0] = serialisedOptimisationProblem;
}
