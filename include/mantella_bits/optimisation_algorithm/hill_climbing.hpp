/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct hill_climbing : optimisation_algorithm<T, number_of_dimensions> {
  using typename optimisation_algorithm<T, number_of_dimensions>::state_type;
  using typename optimisation_algorithm<T, number_of_dimensions>::value_type;
  
  T minimal_stepsize;
  T maximal_stepsize;
  
  constexpr hill_climbing() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr hill_climbing<T, number_of_dimensions>::hill_climbing() noexcept 
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
    "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow center), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*."
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
  mant::hill_climbing<value_type, number_of_dimensions> hill_climbing;
  mant::optimisation_algorithm_state<value_type, number_of_dimensions> hill_climbing_state;
  
  SECTION("Default values") {
    CHECK(hill_climbing.minimal_stepsize == 0.0);
    CHECK(hill_climbing.maximal_stepsize == 0.1);
  }
  
  SECTION("Next parameters functions") {
    CHECK(hill_climbing.next_parameters_functions.size() == 1);
    CHECK(std::get<1>(hill_climbing.next_parameters_functions.at(0)) == "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow center), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*.");
    
    hill_climbing.active_dimensions = {0, 2};
    hill_climbing.minimal_stepsize = 0.2;
    hill_climbing.maximal_stepsize = 0.5;
    hill_climbing_state.parameters.resize(2);
    hill_climbing_state.best_found_parameter = {0.2, 0.1, 0.5};
    
    std::get<0>(hill_climbing.next_parameters_functions.at(0))(hill_climbing_state);
    
    CHECK(hill_climbing_state.parameters.size() == 2);
    // Checks that the parameter's distance from *best_found_parameter* is within
    // [*minimal_stepsize*, *maximal_stepsize*].
    for (const auto& parameter : hill_climbing_state.parameters) {
      auto&& length = std::sqrt(std::inner_product(
        parameter.cbegin(), std::next(parameter.cbegin(), hill_climbing.active_dimensions.size()),
        hill_climbing_state.best_found_parameter.cbegin(),
        0.0,
        std::plus<double>(),
        [](const auto parameter_element, const auto best_found_parameter_element) {
          return std::pow(parameter_element - best_found_parameter_element, 2.0);
        }));
      CHECK(length >= hill_climbing.minimal_stepsize);
      CHECK(length <= hill_climbing.maximal_stepsize);
    }
  }
}
#endif





