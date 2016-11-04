/**
Optimiser
=========

.. cpp:class:: optimiser

  .. versionadded:: 1.0.0 

  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  
  Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. 

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
    
        Constructor
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
    
.. include:: optimiser/hill_climbing.include
.. include:: optimiser/hooke_jeeves_algorithm.include
.. include:: optimiser/nelder_mead_method.include
.. include:: optimiser/particle_swarm_optimisation.include
.. include:: optimiser/random_search.include
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
TEST_CASE("optimiser", "[optimiser]") {
  const mant::optimiser<double, 3> optimiser;
  
  CHECK(optimiser.acceptable_objective_value == -std::numeric_limits<double>::infinity());
  CHECK(optimiser.maximal_evaluations == 3000);
  CHECK(optimiser.maximal_duration == std::chrono::seconds(10));
  CHECK(optimiser.active_dimensions == std::vector<unsigned>({0, 1, 2}));
}
#endif
