/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct hill_climbing : optimisation_algorithm<T, number_of_dimensions> {
  T minimal_stepsize;
  T maximal_stepsize;
  
  hill_climbing() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
hill_climbing<T, number_of_dimensions>::hill_climbing() noexcept 
    : optimisation_algorithm<T, number_of_dimensions>() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  this->next_parameters_functions = {{
    [this](auto& state) {
      assert(minimal_stepsize <= maximal_stepsize);
       
      std::generate(
        state.parameters.begin(), state.parameters.end(),
        [this, &state]() {
          return random_neighbour(
            state.best_found_parameter,
            minimal_stepsize, maximal_stepsize,
            this->active_dimensions.size());
        }
      );
    },
    "Hill climbing next parameters"
  }};
  
  this->minimal_stepsize = T(0.0);
  this->maximal_stepsize = T(0.1);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("hill_climbing", "[hill_climbing]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::hill_climbing<value_type, number_of_dimensions> hill_climbing;
  const mant::optimisation_algorithm_state<value_type, number_of_dimensions> hill_climbing_state;
  
  CHECK(hill_climbing.minimal_stepsize == 0.0);
  CHECK(hill_climbing.maximal_stepsize == 0.1);
}
#endif





