/**

Optimisation result
===================

.. cpp:class:: optimise_result

  .. versionadded:: 1.0.0 

  .. list-table:: Template parameters
    :widths: 27 73

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``unsigned``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<unsigned>::max()]``.

  .. list-table:: Member variables
    :widths: 27 73
  
    * - parameter
    
        ``std::array<T, N>``
      - Lorem ipsum dolor sit amet
  
    * - objective_value
    
        ``T``
      - Lorem ipsum dolor sit amet
  
    * - evaluations
    
        ``unsigned``
      - Lorem ipsum dolor sit amet
  
    * - duration
    
        ``std::chrono::nanoseconds``
      - Lorem ipsum dolor sit amet
*/
template <typename T, unsigned N>
struct optimise_result {
  std::array<T, N> parameter{};
  T objective_value = std::numeric_limits<T>::infinity();
  unsigned evaluations = 0;
  std::chrono::nanoseconds duration = std::chrono::nanoseconds(0);
};

/**
Optimiser
=========

.. cpp:class:: optimiser

  .. versionadded:: 1.0.0 

  .. list-table:: Template parameters
    :widths: 27 73
    
    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``unsigned``
      - Lorem ipsum dolor sit amet
  
  .. list-table:: Member variables
    :widths: 27 73
    
    * - optimisation_function
        
        ``std::function``
      - Lorem ipsum dolor sit amet
          
    * - maximal_evaluations
        
        ``unsigned``
      - Lorem ipsum dolor sit amet
        
    * - maximal_duration
        
        ``std::chrono::nanoseconds``
      - Lorem ipsum dolor sit amet
        
    * - active_dimensions
        
        ``std::vector<unsigned>``
      - Lorem ipsum dolor sit amet

  .. list-table:: Member functions
    :widths: 27 73
    
    * - optimiser
    
        (Constructor)
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
    
.. toctree::
  
  optimiser/hill_climbing
  optimiser/hooke_jeeves_algorithm
  optimiser/nelder_mead_method
  optimiser/particle_swarm_optimisation
  optimiser/random_search
*/
template <typename T, unsigned N>
struct optimiser {
  std::function<optimise_result<T, N>(
      const mant::problem<T, N>& problem,
      const std::vector<std::array<T, N>>& initial_parameters)> optimisation_function;
  
  T acceptable_objective_value;
  unsigned maximal_evaluations;
  std::chrono::nanoseconds maximal_duration;
  
  std::vector<unsigned> active_dimensions;
  
  optimiser() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
optimiser<T, N>::optimiser() noexcept 
  : acceptable_objective_value(-std::numeric_limits<T>::infinity()),
    maximal_evaluations(1'000 * N),
    maximal_duration(std::chrono::seconds(10)) {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(N > 0, "");
  
  active_dimensions.resize(N);
  std::iota(active_dimensions.begin(), active_dimensions.end(), 0);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimise_result", "[optimiser][optimise_result]") {
  constexpr mant::optimise_result<double, 3> result{};
  
  CHECK(result.objective_value == std::numeric_limits<double>::infinity());
  CHECK(result.evaluations == 0);
  CHECK(result.duration == std::chrono::nanoseconds(0));
}

TEST_CASE("optimiser", "[optimiser]") {
  const mant::optimiser<double, 3> optimiser;
  
  CHECK(optimiser.acceptable_objective_value == -std::numeric_limits<double>::infinity());
  CHECK(optimiser.maximal_evaluations == 3000);
  CHECK(optimiser.maximal_duration == std::chrono::seconds(10));
  CHECK(optimiser.active_dimensions == std::vector<unsigned>({0, 1, 2}));
}
#endif
