/**
Optimiser
=========

<<<<<<< HEAD
Optimisation result
===================

.. cpp:class:: template<T, N> optimise_result

  **Template parameters**
  
    - **T** - A floating point type
    - **N** - The (``unsigned``) number of dimensions 
  
  .. cpp:member:: std::array<T, N> parameter
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: T objective_value
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: unsigned evaluations
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::chrono::nanoseconds duration
  
    Lorem ipsum dolor sit amet
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

.. cpp:class:: template<T, N> optimiser

  **Template parameters**
  
    - **T** - A floating point type

      - Lorem ipsum dolor sit amet
    
    - **N** - The (``unsigned``) number of dimensions 

      - Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::function optimisation_function
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: T acceptable_objective_value
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: unsigned maximal_evaluations
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::chrono::nanoseconds maximal_duration
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::vector<unsigned> active_dimensions
  
    Lorem ipsum dolor sit amet
      
  .. cpp:function:: optimiser()
  
    Lorem ipsum dolor sit amet
    
.. toctree::
  
  optimiser/hill_climbing
  optimiser/hooke_jeeves_algorithm
  optimiser/nelder_mead_method
  optimiser/particle_swarm_optimisation
  optimiser/random_search
*/
template <typename T, unsigned N>
=======
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
        
        ``std::size_t``
      - Lorem ipsum dolor sit amet
  
  .. list-table:: Member variables
    :widths: 27 73
    
    * - optimisation_function
        
        ``std::function``
      - Lorem ipsum dolor sit amet
          
    * - maximal_evaluations
        
        ``std::size_t``
      - Lorem ipsum dolor sit amet
        
    * - maximal_duration
        
        ``std::chrono::nanoseconds``
      - Lorem ipsum dolor sit amet
        
    * - active_dimensions
        
        ``std::vector<std::size_t>``
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
template <typename T, std::size_t N>
>>>>>>> master
struct optimiser {
  std::function<optimise_result<T, N>(
      const mant::problem<T, N>& problem,
      const std::vector<std::array<T, N>>& initial_parameters)> optimisation_function;
  
  T acceptable_objective_value;
  std::size_t maximal_evaluations;
  std::chrono::nanoseconds maximal_duration;
  
  std::vector<std::size_t> active_dimensions;
  
  optimiser() noexcept;
};

//
// Implementation
//

<<<<<<< HEAD
template <typename T, unsigned N>
=======
template <typename T, std::size_t N>
>>>>>>> master
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
<<<<<<< HEAD
TEST_CASE("optimise_result", "[optimiser][optimise_result]") {
  constexpr mant::optimise_result<double, 3> result{};
  
  CHECK(result.objective_value == std::numeric_limits<double>::infinity());
  CHECK(result.evaluations == 0);
  CHECK(result.duration == std::chrono::nanoseconds(0));
}

=======
>>>>>>> master
TEST_CASE("optimiser", "[optimiser]") {
  const mant::optimiser<double, 3> optimiser;
  
  CHECK(optimiser.acceptable_objective_value == -std::numeric_limits<double>::infinity());
  CHECK(optimiser.maximal_evaluations == 3000);
  CHECK(optimiser.maximal_duration == std::chrono::seconds(10));
  CHECK(optimiser.active_dimensions == std::vector<std::size_t>({0, 1, 2}));
}
#endif
