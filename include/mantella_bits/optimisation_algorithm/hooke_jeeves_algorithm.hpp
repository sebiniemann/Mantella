#pragma once

/**

*/

template <typename T, std::size_t number_of_dimensions>
struct hooke_jeeves_algorithm_state : optimisation_algorithm_state<T, number_of_dimensions> {
  T step_size;
  
  constexpr hooke_jeeves_algorithm_state() noexcept;
};

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2 = hooke_jeeves_algorithm_state>
struct hooke_jeeves_algorithm : optimisation_algorithm<T1, number_of_dimensions, T2> {
  T1 initial_stepsize;
  T1 stepsize_decrease;
  
  constexpr hooke_jeeves_algorithm() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr hooke_jeeves_algorithm_state<T, number_of_dimensions>::hooke_jeeves_algorithm_state() noexcept
    : optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() {
  step_size = T(1.0);
}

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2>
constexpr hooke_jeeves_algorithm<T1, number_of_dimensions, T2>::hooke_jeeves_algorithm() noexcept 
    : optimisation_algorithm<T1, number_of_dimensions, T2>() {
  this->initialising_functions = {{
    [this](
        auto state) {
      state.step_size = initial_stepsize;
      
      return state;
    },
    "Sets *step_size* to *initial_stepsize*."
  }};
  
  this->next_parameters_functions = {{
    [this](
        auto state) {
      if (state.stagnating_number_of_iterations > 0) {
        state.step_size /= stepsize_decrease;
      }

      return state;
    },
    "Divides *step_size* by *stepsize_decrease*, if *best_found_objective_value* did not improved in the previous iteration."
  }, {
    [this](
        auto state) {
      state.parameters.resize(2 * this->active_dimensions.size());
      
      for (std::size_t n = 0; n < state.parameters.size() / 2; ++n) {
        state.parameters.at(2 * n) = state.best_found_parameter;
        state.parameters.at(2 * n).at(n) += state.step_size;
        state.parameters.at(2 * n + 1) = state.best_found_parameter;
        state.parameters.at(2 * n + 1).at(n) -= state.step_size;
      }

      return state;
    },
    "Generates all parameters, such that for each one only a single dimension differs from *best_found_parameter*, with distance *step_size*."
  }};
  
  this->initial_stepsize = T1(1.0);
  this->stepsize_decrease = T1(2.0);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("hooke_jeeves_algorithm", "[hooke_jeeves_algorithm]") {
  
}
#endif
