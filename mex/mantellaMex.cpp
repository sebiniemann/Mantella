// C++ standard library
#include <cassert>

// Matlab
#include "mantellaMex.hpp"

#include <mex.h>
#if !defined(HAVE_OCTAVE)
#include <matrix.h>
#endif

// Mantella
#include <mantella>

#if defined(HAVE_OCTAVE)
bool mxIsScalar(
    const mxArray* data) {
  return data != nullptr && mxGetNumberOfElements(data) == 1;
}

mxArray* mxCreateUninitNumericMatrix(
    const mwSize numberOfRows,
    const mwSize numberOfColumns,
    const mxClassID classId,
    const mxComplexity complexity) {
  return mxCreateNumericMatrix(numberOfRows, numberOfColumns, classId, complexity);
}
#endif

#if !defined(HAVE_OCTAVE)
ConsoleStreamBuffer::int_type ConsoleStreamBuffer::overflow(
    ConsoleStreamBuffer::int_type character) {
  if (character != traits_type::eof()) {
    mexPrintf("%.1s", &character);
  }

  return 1;
}

std::streamsize ConsoleStreamBuffer::xsputn(
    const char* string,
    std::streamsize stringSize) {
  mexPrintf("%.*s", stringSize, string);
  return stringSize;
}
#endif

arma::mat getRealMatrix(
    const mxArray* data) {
  if (data == nullptr) {
    mexErrMsgTxt("getRealMatrix: The data must exist, i.e. not point to a null pointer.");
  } else if (mxIsSparse(data)) {
    mexErrMsgTxt("getRealMatrix: The data must point to a dense (not sparse) matrix.");
  } else if (!mxIsEmpty(data) && mxGetNumberOfDimensions(data) != 2) {
    mexErrMsgTxt("getRealMatrix: The data must point to a 2-dimensional matrix (if filled).");
  } else if (!mxIsDouble(data)) {
    mexErrMsgTxt("getRealMatrix: The data point to double precision floating point values.");
  }

  return arma::mat(static_cast<double*>(mxGetData(data)), mxGetM(data), mxGetN(data));
}

mxArray* getMxArray(
    const arma::mat& data) {
  mxArray* matrix = mxCreateUninitNumericMatrix(static_cast<mwSize>(data.n_rows), static_cast<mwSize>(data.n_cols), mxDOUBLE_CLASS, mxREAL);

  if (matrix == nullptr) {
    mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
  } else {
    std::copy(data.begin(), data.end(), mxGetPr(matrix));
  }

  return matrix;
}

arma::vec getRealVector(
    const mxArray* data) {
  if (data == nullptr) {
    mexErrMsgTxt("getRealVector: The data must exist, i.e. not point to a null pointer.");
  } else if (mxIsSparse(data)) {
    mexErrMsgTxt("getRealVector: The data must point to a dense (not sparse) matrix.");
  } else if (!mxIsEmpty(data) && !(mxGetNumberOfDimensions(data) == 2 && (mxGetM(data) == 1 || mxGetN(data) == 1))) {
    mexErrMsgTxt("getRealVector: The data must point to a vector (if filled).");
  } else if (!mxIsDouble(data)) {
    mexErrMsgTxt("getRealVector: The data point to double precision floating point values.");
  }

  return arma::vec(static_cast<double*>(mxGetData(data)), mxGetNumberOfElements(data));
}

mxArray* getMxArray(
    const arma::vec& data) {
  mxArray* vector = mxCreateUninitNumericMatrix(static_cast<mwSize>(data.n_elem), 1, mxDOUBLE_CLASS, mxREAL);

  if (vector == nullptr) {
    mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
  } else {
    std::copy(data.begin(), data.end(), mxGetPr(vector));
  }

  return vector;
}

mxArray* getMxArray(
    const arma::rowvec& data) {
  mxArray* vector = mxCreateUninitNumericMatrix(1, static_cast<mwSize>(data.n_elem), mxDOUBLE_CLASS, mxREAL);

  if (vector == nullptr) {
    mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
  } else {
    std::copy(data.begin(), data.end(), mxGetPr(vector));
  }

  return vector;
}

arma::uvec getIntegerVector(
    const mxArray* data) {
  if (data == nullptr) {
    mexErrMsgTxt("getIntegerVector: The data must exist, i.e. not point to a null pointer.");
  } else if (mxIsSparse(data)) {
    mexErrMsgTxt("getIntegerVector: The data must point to a dense (not sparse) matrix.");
  } else if (!mxIsEmpty(data) && !(mxGetNumberOfDimensions(data) == 2 && (mxGetM(data) == 1 || mxGetN(data) == 1))) {
    mexErrMsgTxt("getIntegerVector: The data must point to a vector (if filled).");
  } else if (!mxIsDouble(data)) {
    // Since Matlab stores any data per default as double precision point values, we expect this even for integer types.
    mexErrMsgTxt("getIntegerVector: The data point to double precision floating point values.");
  }

  return arma::conv_to<arma::uvec>::from(arma::vec(static_cast<double*>(mxGetData(data)), mxGetNumberOfElements(data)));
}

mxArray* getMxArray(
    const arma::uvec& data) {
  mxArray* vector = mxCreateUninitNumericMatrix(static_cast<mwSize>(data.n_elem), 1, mxDOUBLE_CLASS, mxREAL);

  if (vector == nullptr) {
    mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
  } else {
    std::copy(data.begin(), data.end(), mxGetPr(vector));
  }

  return vector;
}

double getRealScalar(
    const mxArray* data) {
  if (data == nullptr) {
    mexErrMsgTxt("getRealScalar: The data must exist, i.e. not point to a null pointer.");
  } else if (!mxIsScalar(data)) {
    mexErrMsgTxt("getRealScalar: The data must point to a scalar.");
  } else if (!mxIsDouble(data)) {
    mexErrMsgTxt("getRealScalar: The data point to double precision floating point values.");
  }

  return mxGetScalar(data);
}

mxArray* getMxArray(
    const double data) {
  mxArray* scalar = mxCreateUninitNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);

  if (scalar == nullptr) {
    mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
  } else {
    *mxGetPr(scalar) = data;
  }

  return scalar;
}

arma::uword getIntegerScalar(
    const mxArray* data) {
  if (data == nullptr) {
    mexErrMsgTxt("getIntegerScalar: The data must exist, i.e. not point to a null pointer.");
  } else if (!mxIsScalar(data)) {
    mexErrMsgTxt("getIntegerScalar: The data must point to a scalar.");
  } else if (!mxIsDouble(data)) {
    // Since Matlab stores any data per default as double precision point values, we expect this even for integer types.
    mexErrMsgTxt("getIntegerScalar: The data point to double precision floating point values.");
  }

  return static_cast<arma::uword>(mxGetScalar(data));
}

mxArray* getMxArray(
    const arma::uword data) {
  mxArray* scalar = mxCreateUninitNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);

  if (scalar == nullptr) {
    mexErrMsgTxt("getMxArray: Not enough free heap space (e.g. memory) to allocate.");
  } else {
    *mxGetPr(scalar) = static_cast<double>(data);
  }

  return scalar;
}

std::shared_ptr<mant::OptimisationProblem> getOptimisationProblem(
    const mxArray* serialisedOptimisationProblem) {
  std::shared_ptr<mant::OptimisationProblem> optimisationProblem;

  mxArray* functionHandle = mxGetField(serialisedOptimisationProblem, 0, "functionHandle");
  if (mxIsFunctionHandle(functionHandle)) {
    const arma::uword numberOfDimensions = getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"));
    optimisationProblem = std::shared_ptr<mant::OptimisationProblem>(new mant::OptimisationProblem(2));
    optimisationProblem->setObjectiveFunctions(
        {{[numberOfDimensions, functionHandle](
              const arma::vec& parameter_) {
            assert(parameter_.n_elem == numberOfDimensions);

            mxArray* functionInput[2] = {functionHandle, getMxArray(parameter_)};
            mxArray* functionOutput[1] = {mxCreateDoubleMatrix(1, 1, mxREAL)};
            mexCallMATLAB(1, functionOutput, 2, functionInput, "feval");
            return getRealScalar(functionOutput[0]);
          },
          "Matlab function handle"}});
  } else if (mxIsChar(functionHandle)) {
    std::string functionName = std::string(mxArrayToString(functionHandle));
    if (functionName == "BBOB Attractive Sector Function (f6)") {
      std::shared_ptr<bbob::AttractiveSectorFunction> attractiveSectorFunction(new bbob::AttractiveSectorFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      attractiveSectorFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = attractiveSectorFunction;
    } else if (functionName == "BBOB Bent Cigar Function (f12)") {
      std::shared_ptr<bbob::BentCigarFunction> bentCigarFunction(new bbob::BentCigarFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      bentCigarFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = bentCigarFunction;
    } else if (functionName == "BBOB Büche-Rastrigin Function (f4)") {
      optimisationProblem = std::shared_ptr<mant::bbob::BuecheRastriginFunction>(new mant::bbob::BuecheRastriginFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Composite Griewank Rosenbrock Function F8F2 (f19)") {
      optimisationProblem = std::shared_ptr<mant::bbob::CompositeGriewankRosenbrockFunctionF8F2>(new mant::bbob::CompositeGriewankRosenbrockFunctionF8F2(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Different Powers Function (f14)") {
      optimisationProblem = std::shared_ptr<mant::bbob::DifferentPowersFunction>(new mant::bbob::DifferentPowersFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Discus Function (f11)") {
      optimisationProblem = std::shared_ptr<mant::bbob::DiscusFunction>(new mant::bbob::DiscusFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Ellipsoidal Function (f2)") {
      optimisationProblem = std::shared_ptr<mant::bbob::EllipsoidalFunction>(new mant::bbob::EllipsoidalFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Ellipsoidal Function, rotated (f10)") {
      optimisationProblem = std::shared_ptr<mant::bbob::EllipsoidalFunctionRotated>(new mant::bbob::EllipsoidalFunctionRotated(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Gallagher's Gaussian 101-me Peaks Function (f21)") {
      std::shared_ptr<bbob::GallaghersGaussian101mePeaksFunction> gallaghersGaussian101mePeaksFunction(new bbob::GallaghersGaussian101mePeaksFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      gallaghersGaussian101mePeaksFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      gallaghersGaussian101mePeaksFunction->localParameterTranslations_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "localParameterTranslations"));
      gallaghersGaussian101mePeaksFunction->localParameterConditionings_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "localParameterConditionings"));
      optimisationProblem = gallaghersGaussian101mePeaksFunction;
    } else if (functionName == "BBOB Gallagher's Gaussian 21-hi Peaks Function (f22)") {
      std::shared_ptr<bbob::GallaghersGaussian21hiPeaksFunction> gallaghersGaussian21hiPeaksFunction(new bbob::GallaghersGaussian21hiPeaksFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      gallaghersGaussian21hiPeaksFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      gallaghersGaussian21hiPeaksFunction->localParameterTranslations_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "localParameterTranslations"));
      gallaghersGaussian21hiPeaksFunction->localParameterConditionings_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "localParameterConditionings"));
      optimisationProblem = gallaghersGaussian21hiPeaksFunction;
    } else if (functionName == "BBOB Katsuura Function (f23)") {
      std::shared_ptr<bbob::KatsuuraFunction> katsuuraFunction(new bbob::KatsuuraFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      katsuuraFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = katsuuraFunction;
    } else if (functionName == "BBOB Linear Slope (f5)") {
      optimisationProblem = std::shared_ptr<mant::bbob::LinearSlope>(new mant::bbob::LinearSlope(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Lunacek bi-Rastrigin Function (f24)") {
      std::shared_ptr<bbob::LunacekBiRastriginFunction> lunacekBiRastriginFunction(new bbob::LunacekBiRastriginFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      lunacekBiRastriginFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      lunacekBiRastriginFunction->rotationR_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationR"));
      optimisationProblem = lunacekBiRastriginFunction;
    } else if (functionName == "BBOB Rastrigin Function (f3)") {
      optimisationProblem = std::shared_ptr<mant::bbob::RastriginFunction>(new mant::bbob::RastriginFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Rastrigin Function, rotated (f15)") {
      std::shared_ptr<bbob::RastriginFunctionRotated> rastriginFunctionRotated(new bbob::RastriginFunctionRotated(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      rastriginFunctionRotated->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      rastriginFunctionRotated->rotationR_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationR"));
      optimisationProblem = rastriginFunctionRotated;
    } else if (functionName == "BBOB Rosenbrock Function (f8)") {
      optimisationProblem = std::shared_ptr<mant::bbob::RosenbrockFunction>(new mant::bbob::RosenbrockFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Rosenbrock Function, rotated (f9)") {
      optimisationProblem = std::shared_ptr<mant::bbob::RosenbrockFunctionRotated>(new mant::bbob::RosenbrockFunctionRotated(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Schaffers F7 Function (f17)") {
      std::shared_ptr<bbob::SchaffersF7Function> schaffersF7Function(new bbob::SchaffersF7Function(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      schaffersF7Function->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = schaffersF7Function;
    } else if (functionName == "BBOB Schaffers F7 Function, ill-conditioned (f18)") {
      std::shared_ptr<bbob::SchaffersF7FunctionIllConditioned> schaffersF7FunctionIllConditioned(new bbob::SchaffersF7FunctionIllConditioned(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      schaffersF7FunctionIllConditioned->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = schaffersF7FunctionIllConditioned;
    } else if (functionName == "BBOB Schwefel Function (f20)") {
      optimisationProblem = std::shared_ptr<mant::bbob::SchwefelFunction>(new mant::bbob::SchwefelFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Sharp Ridge Function (f13)") {
      std::shared_ptr<bbob::SharpRidgeFunction> sharpRidgeFunction(new bbob::SharpRidgeFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      sharpRidgeFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = sharpRidgeFunction;
    } else if (functionName == "BBOB Sphere Function (f1)") {
      optimisationProblem = std::shared_ptr<mant::bbob::SphereFunction>(new mant::bbob::SphereFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
    } else if (functionName == "BBOB Step Ellipsoidal Function (f7)") {
      std::shared_ptr<bbob::StepEllipsoidalFunction> stepEllipsoidalFunction(new bbob::StepEllipsoidalFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      stepEllipsoidalFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      optimisationProblem = stepEllipsoidalFunction;
    } else if (functionName == "BBOB Weierstrass Function (f16)") {
      std::shared_ptr<bbob::WeierstrassFunction> weierstrassFunction(new bbob::WeierstrassFunction(getIntegerScalar(mxGetField(serialisedOptimisationProblem, 0, "numberOfDimensions"))));
      weierstrassFunction->rotationQ_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationQ"));
      weierstrassFunction->rotationR_ = getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "rotationR"));
      optimisationProblem = weierstrassFunction;
    } else if (functionName == "KRM Parallel Kinematic Machine 3PRPR") {
      optimisationProblem = std::shared_ptr<mant::krm::ParallelKinematicMachine3PRPR>(new mant::krm::ParallelKinematicMachine3PRPR());
    } else if (functionName == "KRM Parallel Kinematic Machine 3PRRR") {
      optimisationProblem = std::shared_ptr<mant::krm::ParallelKinematicMachine3PRRR>(new mant::krm::ParallelKinematicMachine3PRRR());
    } else if (functionName == "KRM Parallel Kinematic Machine 6PRUS") {
      optimisationProblem = std::shared_ptr<mant::krm::ParallelKinematicMachine6PRUS>(new mant::krm::ParallelKinematicMachine6PRUS());
    } else if (functionName == "KRM Parallel Kinematic Machine 6PUPS") {
      optimisationProblem = std::shared_ptr<mant::krm::ParallelKinematicMachine6PUPS>(new mant::krm::ParallelKinematicMachine6PUPS());
    } else {
      mexErrMsgTxt(std::string("deserialise: There is no supported optimisation problem named '" + functionName + "'.").c_str());
    }
  } else {
    mexErrMsgTxt("deserialise: The function handle must either be a string or an actual function handle.");
  }

  optimisationProblem->setLowerBounds(getRealVector(mxGetField(serialisedOptimisationProblem, 0, "lowerBounds")));
  optimisationProblem->setUpperBounds(getRealVector(mxGetField(serialisedOptimisationProblem, 0, "upperBounds")));
  optimisationProblem->setParameterPermutation(getIntegerVector(mxGetField(serialisedOptimisationProblem, 0, "parameterPermutation")));
  optimisationProblem->setParameterScaling(getRealVector(mxGetField(serialisedOptimisationProblem, 0, "parameterScaling")));
  optimisationProblem->setParameterTranslation(getRealVector(mxGetField(serialisedOptimisationProblem, 0, "parameterTranslation")));
  optimisationProblem->setParameterRotation(getRealMatrix(mxGetField(serialisedOptimisationProblem, 0, "parameterRotation")));
  optimisationProblem->setMinimalParameterDistance(getRealVector(mxGetField(serialisedOptimisationProblem, 0, "minimalParameterDistance")));
  optimisationProblem->setObjectiveValueScaling(getRealScalar(mxGetField(serialisedOptimisationProblem, 0, "objectiveValueScaling")));
  optimisationProblem->setObjectiveValueTranslation(getRealScalar(mxGetField(serialisedOptimisationProblem, 0, "objectiveValueTranslation")));

  return optimisationProblem;
}

mxArray* getMxArray(
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

  // The `functionHandle` field needs to be filled within the `mexFunction` function.
  // It was just added to ensure that its the first field.
  mxSetField(serialisedOptimisationProblem, 0, "numberOfDimensions", getMxArray(optimisationProblem.numberOfDimensions_));
  mxSetField(serialisedOptimisationProblem, 0, "lowerBounds", getMxArray(optimisationProblem.getLowerBounds()));
  mxSetField(serialisedOptimisationProblem, 0, "upperBounds", getMxArray(optimisationProblem.getUpperBounds()));
  mxSetField(serialisedOptimisationProblem, 0, "parameterPermutation", getMxArray(optimisationProblem.getParameterPermutation()));
  mxSetField(serialisedOptimisationProblem, 0, "parameterScaling", getMxArray(optimisationProblem.getParameterScaling()));
  mxSetField(serialisedOptimisationProblem, 0, "parameterTranslation", getMxArray(optimisationProblem.getParameterTranslation()));
  mxSetField(serialisedOptimisationProblem, 0, "parameterRotation", getMxArray(optimisationProblem.getParameterRotation()));
  mxSetField(serialisedOptimisationProblem, 0, "minimalParameterDistance", getMxArray(optimisationProblem.getMinimalParameterDistance()));
  mxSetField(serialisedOptimisationProblem, 0, "objectiveValueScaling", getMxArray(optimisationProblem.getObjectiveValueScaling()));
  mxSetField(serialisedOptimisationProblem, 0, "objectiveValueTranslation", getMxArray(optimisationProblem.getObjectiveValueTranslation()));

  return serialisedOptimisationProblem;
}

mxArray* getMxArray(
    const mant::BlackBoxOptimisationBenchmark& optimisationProblem) {
  return getMxArray(static_cast<mant::OptimisationProblem>(optimisationProblem));
}

mxArray* getMxArray(
    const mant::KinematicallyRedundantMachines& optimisationProblem) {
  return getMxArray(static_cast<mant::OptimisationProblem>(optimisationProblem));
}

std::shared_ptr<mant::OptimisationAlgorithm> getOptimisationAlgorithm(
    const mxArray* serialisedOptimisationAlgorithm) {
  std::shared_ptr<mant::OptimisationAlgorithm> optimisationAlgorithm;

  mxArray* functionHandle = mxGetField(serialisedOptimisationAlgorithm, 0, "functionHandle");
  if (mxIsChar(functionHandle)) {
    std::string functionName = std::string(mxArrayToString(functionHandle));
    if (functionName == "Differential evolution") {
      std::shared_ptr<mant::DifferentialEvolution> differentialEvolution(new mant::DifferentialEvolution());
      differentialEvolution->setScalingFactor(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "scalingFactor")));
      optimisationAlgorithm = differentialEvolution;
    } else if (functionName == "Hill climbing") {
      std::shared_ptr<mant::HillClimbing> hillClimbing(new mant::HillClimbing());
      hillClimbing->setMinimalStepSize(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "minimalStepSize")));
      hillClimbing->setMaximalStepSize(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalStepSize")));
      optimisationAlgorithm = hillClimbing;
    } else if (functionName == "Hooke-Jeeves algorithm") {
      std::shared_ptr<mant::HookeJeevesAlgorithm> hookeJeevesAlgorithm(new mant::HookeJeevesAlgorithm());
      hookeJeevesAlgorithm->setInitialStepSize(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "initialStepSize")));
      hookeJeevesAlgorithm->setStepSizeDecrease(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "stepSizeDecrease")));
      optimisationAlgorithm = hookeJeevesAlgorithm;
    } else if (functionName == "Particle swarm optimisation") {
      std::shared_ptr<mant::ParticleSwarmOptimisation> particleSwarmOptimisation(new mant::ParticleSwarmOptimisation());
      particleSwarmOptimisation->setMaximalAcceleration(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalAcceleration")));
      particleSwarmOptimisation->setMaximalLocalAttraction(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalLocalAttraction")));
      particleSwarmOptimisation->setMaximalGlobalAttraction(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalGlobalAttraction")));
      optimisationAlgorithm = particleSwarmOptimisation;
    } else if (functionName == "Random search") {
      optimisationAlgorithm = std::shared_ptr<mant::RandomSearch>(new mant::RandomSearch());
    } else if (functionName == "Simulated annealing") {
      std::shared_ptr<mant::SimulatedAnnealing> simulatedAnnealing(new mant::SimulatedAnnealing());
      simulatedAnnealing->setMinimalStepSize(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "minimalStepSize")));
      simulatedAnnealing->setMaximalStepSize(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalStepSize")));
      optimisationAlgorithm = simulatedAnnealing;
    } else {
      mexErrMsgTxt(std::string("deserialise: There is no supported optimisation problem named '" + functionName + "'.").c_str());
    }
  } else {
    mexErrMsgTxt("deserialise: The function handle must be a string.");
  }

  optimisationAlgorithm->setAcceptableObjectiveValue(getRealScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "acceptableObjectiveValue")));
  optimisationAlgorithm->setMaximalNumberOfIterations(getIntegerScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalNumberOfIterations")));
  optimisationAlgorithm->setMaximalDuration(std::chrono::milliseconds(getIntegerScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "maximalDuration"))));
  optimisationAlgorithm->setPopulationSize(getIntegerScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "populationSize")));
  optimisationAlgorithm->setNumberOfCommunicationStalls(getIntegerScalar(mxGetField(serialisedOptimisationAlgorithm, 0, "numberOfCommunicationStalls")));

  return optimisationAlgorithm;
}

mxArray* getMxArray(
    const mant::OptimisationAlgorithm& optimisationAlgorithm) {
  const char* fieldNames[6]{
      "functionHandle",
      "acceptableObjectiveValue",
      "maximalNumberOfIterations",
      "maximalDuration",
      "populationSize",
      "numberOfCommunicationStalls"};
  mxArray* serialisedOptimisationAlgorithm = mxCreateStructMatrix(1, 1, 6, fieldNames);

  // The `functionHandle` field needs to be filled within the `mexFunction` function.
  // It was just added to ensure that its the first field.
  mxSetField(serialisedOptimisationAlgorithm, 0, "acceptableObjectiveValue", getMxArray(optimisationAlgorithm.getAcceptableObjectiveValue()));
  mxSetField(serialisedOptimisationAlgorithm, 0, "maximalNumberOfIterations", getMxArray(optimisationAlgorithm.getMaximalNumberOfIterations()));
  mxSetField(serialisedOptimisationAlgorithm, 0, "maximalDuration", getMxArray(static_cast<arma::uword>(optimisationAlgorithm.getMaximalDuration().count())));
  mxSetField(serialisedOptimisationAlgorithm, 0, "populationSize", getMxArray(optimisationAlgorithm.getPopulationSize()));
  mxSetField(serialisedOptimisationAlgorithm, 0, "numberOfCommunicationStalls", getMxArray(optimisationAlgorithm.getNumberOfCommunicationStalls()));

  return serialisedOptimisationAlgorithm;
}

void initialise() {
  // Handles global variables.
  const mxArray* mantIsCachingSamples = mexGetVariablePtr("base", "mantIsCachingSamples");
  if (mantIsCachingSamples != nullptr) {
    if (!mxIsLogicalScalar(mantIsCachingSamples)) {
      mexErrMsgTxt("handleGlobals: mantIsCachingSamples must be scalar if set.");
    }
    ::mant::isCachingSamples = mxIsLogicalScalarTrue(mantIsCachingSamples);
  }

  const mxArray* mantIsRecordingSampling = mexGetVariablePtr("base", "mantIsRecordingSampling");
  if (mantIsRecordingSampling != nullptr) {
    if (!mxIsLogicalScalar(mantIsRecordingSampling)) {
      mexErrMsgTxt("handleGlobals: mantIsCachingSamples must be scalar if set.");
    }
    ::mant::isRecordingSampling = mxIsLogicalScalarTrue(mantIsRecordingSampling);
  }

  const mxArray* mantIsVerbose = mexGetVariablePtr("base", "mantIsVerbose");
  if (mantIsVerbose != nullptr) {
    if (!mxIsLogicalScalar(mantIsVerbose)) {
      mexErrMsgTxt("handleGlobals: mantIsCachingSamples must be scalar if set.");
    }
    ::mant::isVerbose = mxIsLogicalScalarTrue(mantIsVerbose);
  }

  const mxArray* mantMachinePrecision = mexGetVariablePtr("base", "mantMachinePrecision");
  if (mantMachinePrecision != nullptr) {
    if (!mxIsScalar(mantMachinePrecision)) {
      mexErrMsgTxt("handleGlobals: mantIsCachingSamples must be scalar if set.");
    }
    ::mant::machinePrecision = mxGetScalar(mantMachinePrecision);
  }

#if !defined(HAVE_OCTAVE)
  ConsoleStreamBuffer consoleStreamBuffer;
  std::cout.rdbuf(&consoleStreamBuffer);
#endif
}
