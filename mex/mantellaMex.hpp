#pragma once

// C++ standard library
#include <memory>

// Matlab
#include <mex.h>
#if !defined(HAVE_OCTAVE)
#include <matrix.h>
#endif

// Mantella
#include <mantella>

#if defined(HAVE_OCTAVE)
/** 
  * Adds missing `mxIsScalar` for Octave builds.
  * Returns true if `data` is a scalar and false otherwise.
  */
bool mxIsScalar(
    const mxArray* data);
    
mxArray* mxCreateUninitNumericMatrix(
    const mwSize numberOfRows,
    const mwSize numberOfColumns,
    const mxClassID classId,
    const mxComplexity complexity);
#endif

#if !defined(HAVE_OCTAVE)
/** 
  * Provides a stream buffer to redirect everything send to the stream to Matlab's console.
  */
class ConsoleStreamBuffer : public std::streambuf {
 protected:
  int_type overflow(
      int_type character = traits_type::eof());
};
#endif

arma::mat getRealMatrix(
    const mxArray* data);
mxArray* getMxArray(
    const arma::mat& data);
    
arma::vec getRealVector(
    const mxArray* data);
mxArray* getMxArray(
    const arma::vec& data);
    
mxArray* getMxArray(
    const arma::rowvec& data);
    
arma::uvec getIntegerVector(
    const mxArray* data);
mxArray* getMxArray(
    const arma::uvec& data);
    
double getRealScalar(
    const mxArray* data);
mxArray* getMxArray(
    const double data);

arma::uword getIntegerScalar(
    const mxArray* data);
mxArray* getMxArray(
    const arma::uword data);
    
std::shared_ptr<mant::OptimisationProblem> getOptimisationProblem(
    const mxArray* serialisedOptimisationProblem);
mxArray* getMxArray(
    const mant::OptimisationProblem& optimisationProblem);
    
std::shared_ptr<mant::BlackBoxOptimisationBenchmark> getBlackBoxOptimisationBenchmark(
    const mxArray* serialisedOptimisationProblem);
    
std::shared_ptr<mant::KinematicallyRedundantMachines> getKinematicallyRedundantMachines(
    const mxArray* serialisedOptimisationProblem);

void initialise();

namespace bbob {
  class AttractiveSectorFunction : public mant::bbob::AttractiveSectorFunction {
   public:
    using mant::bbob::AttractiveSectorFunction::AttractiveSectorFunction;
    using mant::bbob::AttractiveSectorFunction::rotationQ_;
  };
  
  class BentCigarFunction : public mant::bbob::BentCigarFunction {
   public:
    using mant::bbob::BentCigarFunction::BentCigarFunction;
    using mant::bbob::BentCigarFunction::rotationQ_;
  };
  
  class GallaghersGaussian101mePeaksFunction : public mant::bbob::GallaghersGaussian101mePeaksFunction {
   public:
    using mant::bbob::GallaghersGaussian101mePeaksFunction::GallaghersGaussian101mePeaksFunction;
    using mant::bbob::GallaghersGaussian101mePeaksFunction::rotationQ_;
    using mant::bbob::GallaghersGaussian101mePeaksFunction::localParameterTranslations_;
    using mant::bbob::GallaghersGaussian101mePeaksFunction::localParameterConditionings_;
  };
  
  class GallaghersGaussian21hiPeaksFunction : public mant::bbob::GallaghersGaussian21hiPeaksFunction {
   public:
    using mant::bbob::GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction;
    using mant::bbob::GallaghersGaussian21hiPeaksFunction::rotationQ_;
    using mant::bbob::GallaghersGaussian21hiPeaksFunction::localParameterTranslations_;
    using mant::bbob::GallaghersGaussian21hiPeaksFunction::localParameterConditionings_;
  };
  
  class KatsuuraFunction : public mant::bbob::KatsuuraFunction {
   public:
    using mant::bbob::KatsuuraFunction::KatsuuraFunction;
    using mant::bbob::KatsuuraFunction::rotationQ_;
  };
  
  class LunacekBiRastriginFunction : public mant::bbob::LunacekBiRastriginFunction {
   public:
    using mant::bbob::LunacekBiRastriginFunction::LunacekBiRastriginFunction;
    using mant::bbob::LunacekBiRastriginFunction::rotationQ_;
    using mant::bbob::LunacekBiRastriginFunction::rotationR_;
  };
  
  class RastriginFunctionRotated : public mant::bbob::RastriginFunctionRotated {
   public:
    using mant::bbob::RastriginFunctionRotated::RastriginFunctionRotated;
    using mant::bbob::RastriginFunctionRotated::rotationQ_;
    using mant::bbob::RastriginFunctionRotated::rotationR_;
  };
  
  class SchaffersF7Function : public mant::bbob::SchaffersF7Function {
   public:
    using mant::bbob::SchaffersF7Function::SchaffersF7Function;
    using mant::bbob::SchaffersF7Function::rotationQ_;
  };
  
  class SchaffersF7FunctionIllConditioned : public mant::bbob::SchaffersF7FunctionIllConditioned {
   public:
    using mant::bbob::SchaffersF7FunctionIllConditioned::SchaffersF7FunctionIllConditioned;
    using mant::bbob::SchaffersF7FunctionIllConditioned::rotationQ_;
  };
  
  class SharpRidgeFunction : public mant::bbob::SharpRidgeFunction {
   public:
    using mant::bbob::SharpRidgeFunction::SharpRidgeFunction;
    using mant::bbob::SharpRidgeFunction::rotationQ_;
  };
  
  class StepEllipsoidalFunction : public mant::bbob::StepEllipsoidalFunction {
   public:
    using mant::bbob::StepEllipsoidalFunction::StepEllipsoidalFunction;
    using mant::bbob::StepEllipsoidalFunction::rotationQ_;
  };
  
  class WeierstrassFunction : public mant::bbob::WeierstrassFunction {
   public:
    using mant::bbob::WeierstrassFunction::WeierstrassFunction;
    using mant::bbob::WeierstrassFunction::rotationQ_;
    using mant::bbob::WeierstrassFunction::rotationR_;
  };
}
