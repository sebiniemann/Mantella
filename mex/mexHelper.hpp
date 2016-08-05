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
#endif

#if !defined(HAVE_OCTAVE)
/** 
  * Provides a stream buffer to redirect everything send to the stream to Matlab's console.
  */
class ConsoleStreamBuffer : public std::streambuf {
 protected:
  int_type overflow(
      int_type character = traits_type::eof());
#endif

  /**
 * Handles 
 */
  void initialise();

  /**
 *
 */
  std::shared_ptr<mant::OptimisationProblem> deserialise(
      const mxArray* serialisedOptimisationProblem);

  /**
 * Converts optimisation problems to Matlab structs.
 */
  mxArray* serialise(
      const mant::OptimisationProblem& optimisationProblem);

  /**
 * Converts black-box optimisation problems to Matlab structs.
 */
  mxArray* serialise(
      const mant::BlackBoxOptimisationBenchmark& optimisationProblem);
