/**
optimise_result
===============

.. cpp:class:: template<T, N> optimise_result

  **Template parameters**
  
    - **T** (``floating point``)

      - Lorem ipsum dolor sit amet
    
    - **N** (``unsigned``)

      - Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::array<T, N> best_parameter
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: T best_objective_value
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: unsigned evaluations
  
    Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::chrono::nanoseconds duration
  
    Lorem ipsum dolor sit amet
*/
template <typename T, unsigned N>
struct optimise_result {
  std::array<T, N> best_parameter{};
  T best_objective_value = std::numeric_limits<T>::infinity();
  unsigned evaluations = 0;
  std::chrono::nanoseconds duration = std::chrono::nanoseconds(0);
};

/**
optimiser
=========

.. cpp:class:: template<T, N> optimiser

  **Template parameters**
  
    - **T** (``floating point``)

      - Lorem ipsum dolor sit amet
    
    - **N** (``unsigned``)

      - Lorem ipsum dolor sit amet
  
  .. cpp:member:: std::function<optimise_result<T, N>(const mant::problem<T, N>& problem, const std::vector<std::array<T, N>>& initial_parameters)> optimisation_function
  
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
  
  CHECK(result.best_objective_value == std::numeric_limits<double>::infinity());
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
