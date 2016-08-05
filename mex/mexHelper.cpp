// C++ standard library
#include <cassert>

// Matlab
#include "mexHelper.hpp"

#include <mex.h>
#if !defined(HAVE_OCTAVE)
#include <matrix.h>
#endif

// Mantella
#include <mantella>

// Added missing `mxIsScalar` for Octave builds.
#if defined(HAVE_OCTAVE)
bool mxIsScalar(
    const mxArray* data) {
  return data != nullptr && mxGetNumberOfElements(data) == 1;
}
#endif

// Prints out inputs to the standard stream on Matlab builds.
// Octave displays everything send to the standard stream as default.
#if !defined(HAVE_OCTAVE)
class ConsoleStreamBuffer : public std::streambuf {
 protected:
  int_type overflow(
      int_type character = traits_type::eof()) {
    if (character != traits_type::eof()) {
      mexPrintf("%.1s", &character);
    }

    return 1;
  }
};
#endif

void initialise() {
  // Handles global variables.
  const mxArray* mantIsCachingSamples = mexGetVariablePtr("base", "mantIsCachingSamples");
  if (mantIsCachingSamples != nullptr) {
    if (!mxIsLogicalScalar(mantIsCachingSamples)) {
      mexErrMsgIdAndTxt("handleGlobals", "mantIsCachingSamples must be scalar if set.");
    }
    ::mant::isCachingSamples = mxIsLogicalScalarTrue(mantIsCachingSamples);
  }

  const mxArray* mantIsRecordingSampling = mexGetVariablePtr("base", "mantIsRecordingSampling");
  if (mantIsRecordingSampling != nullptr) {
    if (!mxIsLogicalScalar(mantIsRecordingSampling)) {
      mexErrMsgIdAndTxt("handleGlobals", "mantIsCachingSamples must be scalar if set.");
    }
    ::mant::isRecordingSampling = mxIsLogicalScalarTrue(mantIsRecordingSampling);
  }

  const mxArray* mantIsVerbose = mexGetVariablePtr("base", "mantIsVerbose");
  if (mantIsVerbose != nullptr) {
    if (!mxIsLogicalScalar(mantIsVerbose)) {
      mexErrMsgIdAndTxt("handleGlobals", "mantIsCachingSamples must be scalar if set.");
    }
    ::mant::isVerbose = mxIsLogicalScalarTrue(mantIsVerbose);
  }

  const mxArray* mantMachinePrecision = mexGetVariablePtr("base", "mantMachinePrecision");
  if (mantMachinePrecision != nullptr) {
    if (!mxIsScalar(mantMachinePrecision)) {
      mexErrMsgIdAndTxt("handleGlobals", "mantIsCachingSamples must be scalar if set.");
    }
    ::mant::machinePrecision = mxGetScalar(mantMachinePrecision);
  }

#if !defined(HAVE_OCTAVE)
  ConsoleStreamBuffer consoleStreamBuffer;
  std::cout.rdbuf(&consoleStreamBuffer);
#endif
}

std::shared_ptr<mant::OptimisationProblem> deserialise(
    const mxArray* serialisedOptimisationProblem) {
  std::shared_ptr<mant::OptimisationProblem> optimisationProblem;

  mxArray* mexFunctionHandle = mxGetField(serialisedOptimisationProblem, 0, "functionHandle");
  if (mxIsFunctionHandle(mexFunctionHandle)) {
    std::cout << "mxIsFunctionHandle" << std::endl;
    optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::OptimisationProblem(2));
    optimisationProblem->setObjectiveFunctions(
        {{[optimisationProblem, mexFunctionHandle](
              const arma::vec& parameter_) {
            assert(parameter_.n_elem == optimisationProblem->numberOfDimensions_);

            mxArray* mexFunctionInput[2];
            mexFunctionInput[0] = mexFunctionHandle;
            mxArray* mexParameter = mxCreateDoubleMatrix(optimisationProblem->numberOfDimensions_, 1, mxREAL);
            std::copy(parameter_.begin(), parameter_.end(), mxGetPr(mexParameter));
            mexFunctionInput[1] = mexParameter;

            mxArray* mexFunctionOutput[1];
            mexFunctionOutput[0] = mxCreateDoubleMatrix(1, 1, mxREAL);

            mexCallMATLAB(1, mexFunctionOutput, 2, mexFunctionInput, "feval");
            return mxGetScalar(mexFunctionOutput[0]);
          },
          "Matlab function handle"}});
    return optimisationProblem;
  } else if (mxIsChar(mexFunctionHandle)) {
    std::cout << "mxIsChar" << std::endl;
    std::string functionName = std::string(mxArrayToString(mexFunctionHandle));
    if (functionName == "BBOB Attractive Sector Function (f6)") {
      std::cout << functionName << std::endl;
      optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::bbob::AttractiveSectorFunction(2));
    } else {
      mexErrMsgIdAndTxt("deserialise", std::string("There is no supported optimisation problem named '" + functionName + "'.").c_str());
    }
  } else {
    mexErrMsgIdAndTxt("deserialise", "The function handle must either be a string or an actual function handle.");
  }
}

mxArray* serialise(
    const mant::OptimisationProblem& optimisationProblem) {
  const char* fieldNames[11]{
      "functionHandle",
      "numberOfDimensions",
      "lowerBounds",
      "upperBounds",
      "parameterPermutation",
      "parameterScaling",
      "parameterTranslation",
      "parameterRotation",
      "minimalParameterDistance",
      "objectiveValueScaling",
      "objectiveValueTranslation"};
  mxArray* serialisedOptimisationProblem = mxCreateStructMatrix(1, 1, 11, fieldNames);

  const auto objectiveFunctions = optimisationProblem.getObjectiveFunctions();
  mxSetField(serialisedOptimisationProblem, 0, "functionHandle", mxCreateString(std::accumulate(objectiveFunctions.begin() + 1, objectiveFunctions.end(), objectiveFunctions.at(0).second, [](const std::string& accumulatedName, const auto& nextFunction) { return accumulatedName + ", " + nextFunction.second; }).c_str()));

  mxArray* mexNumberOfDimensions = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
  mxGetPr(mexNumberOfDimensions)[0] = optimisationProblem.numberOfDimensions_;
  mxSetField(serialisedOptimisationProblem, 0, "numberOfDimensions", mexNumberOfDimensions);

  mxArray* mexLowerBounds = mxCreateDoubleMatrix(optimisationProblem.numberOfDimensions_, 1, mxREAL);
  const arma::vec& lowerBounds = optimisationProblem.getLowerBounds();
  std::copy(lowerBounds.begin(), lowerBounds.end(), mxGetPr(mexLowerBounds));
  mxSetField(serialisedOptimisationProblem, 0, "lowerBounds", mexLowerBounds);

  mxArray* mexUpperBounds = mxCreateDoubleMatrix(optimisationProblem.numberOfDimensions_, 1, mxREAL);
  const arma::vec& upperBounds = optimisationProblem.getUpperBounds();
  std::copy(upperBounds.begin(), upperBounds.end(), mxGetPr(mexUpperBounds));
  mxSetField(serialisedOptimisationProblem, 0, "upperBounds", mexUpperBounds);

  mxArray* mexParameterPermutation = mxCreateDoubleMatrix(optimisationProblem.numberOfDimensions_, 1, mxREAL);
  const arma::uvec& parameterPermutation = optimisationProblem.getParameterPermutation();
  std::copy(parameterPermutation.begin(), parameterPermutation.end(), mxGetPr(mexParameterPermutation));
  mxSetField(serialisedOptimisationProblem, 0, "parameterPermutation", mexParameterPermutation);

  mxArray* mexParameterScaling = mxCreateDoubleMatrix(optimisationProblem.numberOfDimensions_, 1, mxREAL);
  const arma::vec& parameterScaling = optimisationProblem.getParameterScaling();
  std::copy(parameterScaling.begin(), parameterScaling.end(), mxGetPr(mexParameterScaling));
  mxSetField(serialisedOptimisationProblem, 0, "parameterScaling", mexParameterScaling);

  mxArray* mexParameterTranslation = mxCreateDoubleMatrix(optimisationProblem.numberOfDimensions_, 1, mxREAL);
  const arma::vec& parameterTranslation = optimisationProblem.getParameterTranslation();
  std::copy(parameterTranslation.begin(), parameterTranslation.end(), mxGetPr(mexParameterTranslation));
  mxSetField(serialisedOptimisationProblem, 0, "parameterTranslation", mexParameterTranslation);

  mxArray* mexParameterRotation = mxCreateNumericMatrix(optimisationProblem.numberOfDimensions_, optimisationProblem.numberOfDimensions_, mxDOUBLE_CLASS, mxREAL);
  const arma::mat& parameterRotation = optimisationProblem.getParameterRotation();
  std::copy(parameterRotation.begin(), parameterRotation.end(), mxGetPr(mexParameterRotation));
  mxSetField(serialisedOptimisationProblem, 0, "parameterRotation", mexParameterRotation);

  mxArray* mexMinimalParameterDistance = mxCreateDoubleMatrix(optimisationProblem.numberOfDimensions_, 1, mxREAL);
  const arma::vec& minimalParameterDistance = optimisationProblem.getMinimalParameterDistance();
  std::copy(minimalParameterDistance.begin(), minimalParameterDistance.end(), mxGetPr(mexMinimalParameterDistance));
  mxSetField(serialisedOptimisationProblem, 0, "minimalParameterDistance", mexMinimalParameterDistance);

  mxArray* mexObjectiveValueScaling = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
  mxGetPr(mexObjectiveValueScaling)[0] = optimisationProblem.getObjectiveValueScaling();
  mxSetField(serialisedOptimisationProblem, 0, "objectiveValueScaling", mexObjectiveValueScaling);

  mxArray* mexObjectiveValueTranslation = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
  mxGetPr(mexObjectiveValueTranslation)[0] = optimisationProblem.getObjectiveValueTranslation();
  mxSetField(serialisedOptimisationProblem, 0, "objectiveValueTranslation", mexObjectiveValueTranslation);

  return serialisedOptimisationProblem;
}

mxArray* serialise(
    const mant::BlackBoxOptimisationBenchmark& optimisationProblem) {
  return serialise(static_cast<mant::OptimisationProblem>(optimisationProblem));
}
