//------------------------------------------------------------------------------
// Licensed under the MIT License (the "License");
// You may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Main developers:
//   Sebastian Niemann <niemann@sra.uni-hannover.de> (Lead developer)
//   Oskar Pusz
//   Daniel Kiechle
//   Wolff Bock von Wülfingen
//   Philipp Schiffmann
//------------------------------------------------------------------------------
#pragma once

#define MANTELLA_VERSION_MAJOR 0
#define MANTELLA_VERSION_MINOR 40
#define MANTELLA_VERSION_PATCH 0

//
// Configuration
//
// OpenMP support can either be added via CMake, or manually activated by defining the flag below.
// Ensure that the compiler flags and links, required by OpenMP, are added during compilation.
#cmakedefine MANTELLA_SUPPORT_OPENMP

// The maximal number of threads to be supported by Mantella.
// Larger values may result in a greater start up time and decrease efficiency.
// In case `MAXIMAL_NUMBER_OF_THREADS` was not defined before, we fall back to the value below, determined via CMake.
#if !defined(MANTELLA_MAXIMAL_NUMBER_OF_THREADS)
// Clang format interferes with CMake's @-notation.
// clang-format off
#define MANTELLA_MAXIMAL_NUMBER_OF_THREADS @MANTELLA_MAXIMAL_NUMBER_OF_THREADS@
// clang-format on
#endif

// MPI support can either be added via CMake, or manually activated by defining the flag below.
// Ensure that the compiler flags and links, required by MPI, are added during compilation.
#cmakedefine MANTELLA_SUPPORT_MPI

// Whether or not Mantella's unit test code should be added.
// If neither `CATCH_CONFIG_MAIN` nor `CATCH_CONFIG_RUNNER` if defined prior to including Mantella (the first time), `CATCH_CONFIG_MAIN` is defined as default.
#cmakedefine MANTELLA_BUILD_TESTS

# if !defined(MANTELLA_SYMBOL_VERSION)
  #define MANTELLA_SYMBOL_VERSION 0
#endif

// Type definitions
#include <random>
namespace mant {
# if MANTELLA_SYMBOL_VERSION == 0
  inline 
#endif
  namespace v0 {
    // Global random number generator type.
    typedef std::mt19937_64 random_number_generator_t;
  }
}

//
// Dependencies
//
// C++ standard library
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// BLAS
#include <cblas.h>

// LAPACK
#include <lapacke.h>

// OpenMP
#if defined(MANTELLA_SUPPORT_OPENMP)
#include <omp.h> // IWYU pragma: keep
#endif

// MPI
#if defined(MANTELLA_SUPPORT_MPI)
#include <mpi.h> // IWYU pragma: keep
#endif

// Catch
#if defined(MANTELLA_BUILD_TESTS)
  #if !defined(CATCH_CONFIG_MAIN) and !defined(CATCH_CONFIG_RUNNER)
    #define CATCH_CONFIG_MAIN
  #endif
  #include <catch.hpp>
#endif

//
// Mantella
//
// IWYU pragma: begin_exports
namespace mant {
# if MANTELLA_SYMBOL_VERSION == 0
  inline 
#endif
  namespace v0 {
// Parallelisation
//// OpenMP
#include "mantella_bits/parallelisation/thread_number.hpp"
#include "mantella_bits/parallelisation/number_of_threads.hpp"
//// MPI
#include "mantella_bits/parallelisation/node_rank.hpp"
#include "mantella_bits/parallelisation/number_of_nodes.hpp"

// Random number generator
#include "mantella_bits/random_number_generator/random_number_generators.hpp"
#include "mantella_bits/random_number_generator/seed_random_number_generator.hpp"
#include "mantella_bits/random_number_generator/random_seed.hpp"

// Assertions
#include "mantella_bits/assertion/is_approximately_equal.hpp"
#include "mantella_bits/assertion/is_essentially_equal.hpp"
#include "mantella_bits/assertion/is_permutation_vector.hpp"
#include "mantella_bits/assertion/is_rotation_matrix.hpp"
// #include "mantella_bits/assertion/is_uniform.hpp"

// Mathematical functions
//// Algebra
#include "mantella_bits/algebra/polynomial_size.hpp"
#include "mantella_bits/algebra/polynomial.hpp"
//// Combinatorics
#include "mantella_bits/combinatorics/factorial.hpp"
#include "mantella_bits/combinatorics/n_choose_k.hpp"
#include "mantella_bits/combinatorics/second_stirling_number.hpp"
#include "mantella_bits/combinatorics/combinations.hpp"
#include "mantella_bits/combinatorics/multicombinations.hpp"
#include "mantella_bits/combinatorics/two_sets_partitions.hpp"
//// Geometry
#include "mantella_bits/geometry/rotate_2d.hpp"
#include "mantella_bits/geometry/rotate_3d.hpp"
#include "mantella_bits/geometry/circle_sphere_intersections.hpp"
#include "mantella_bits/geometry/sphere_sphere_intersections.hpp"
//// Linear algebra
#include "mantella_bits/linear_algebra/qr_decomposition.hpp"
//// Number analysis
#include "mantella_bits/number_analysis/brent.hpp"
//// Number theory
#include "mantella_bits/number_theory/van_der_corput_sequence.hpp"
#include "mantella_bits/number_theory/halton_sequence.hpp"
#include "mantella_bits/number_theory/hammersley_set.hpp"
//// Probability
#include "mantella_bits/probability/random_permutation_vector.hpp"
#include "mantella_bits/probability/random_rotation_matrix.hpp"
#include "mantella_bits/probability/random_neighbour.hpp"

// Orbital mechanics
#include "mantella_bits/orbital_mechanics/stumpff_function.hpp"

// Machine learning
//// Samples selection
#include "mantella_bits/samples_selection/elitists.hpp"
//// Samples analysis
#include "mantella_bits/samples_analysis/objective_distance_correlation.hpp"
#include "mantella_bits/samples_analysis/lipschitz_continuity.hpp"
#include "mantella_bits/samples_analysis/additive_separability.hpp"

// Optimisation problems
#include "mantella_bits/optimisation_problem.hpp"

// Optimisation
#include "mantella_bits/optimisation/evaluate.hpp"
  }
}
// IWYU pragma: end_exports
