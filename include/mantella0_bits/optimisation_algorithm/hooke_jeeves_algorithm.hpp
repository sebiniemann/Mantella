/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct hooke_jeeves_algorithm_state : optimisation_algorithm_state<T, number_of_dimensions> {
  T stepsize;
  
  hooke_jeeves_algorithm_state() noexcept;
};

/**

*/
template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2 = hooke_jeeves_algorithm_state>
struct hooke_jeeves_algorithm : optimisation_algorithm<T1, number_of_dimensions, T2> {
  T1 initial_stepsize;
  T1 stepsize_decrease;
  
  hooke_jeeves_algorithm() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
hooke_jeeves_algorithm_state<T, number_of_dimensions>::hooke_jeeves_algorithm_state() noexcept
    : optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  stepsize = T(1.0);
}

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
hooke_jeeves_algorithm<T1, number_of_dimensions, T2>::hooke_jeeves_algorithm() noexcept 
    : optimisation_algorithm<T1, number_of_dimensions, T2>() {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<hooke_jeeves_algorithm_state<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  
  this->initialising_functions = {{
    [this](auto& state) {
      state.stepsize = initial_stepsize;
    },
    "Hooke-Jeeves initialising"
  }};
  
  this->next_parameters_functions = {{
    [this](
        auto& state) {
      if (state.stagnating_number_of_iterations > 0) {
        state.stepsize /= stepsize_decrease;
      }
    },
    "Hooke-Jeeves next parameters #1"
  }, {
    [this](auto& state) {
      state.parameters.resize(2 * this->active_dimensions.size());
      std::fill(
        state.parameters.begin(), state.parameters.end(),
        state.best_found_parameter);
      
      for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
        state.parameters.at(2 * n).at(n) += state.stepsize;
        state.parameters.at(2 * n + 1).at(n) -= state.stepsize;
      }
    },
    "Hooke-Jeeves next parameters #2"
  }};
  
  this->initial_stepsize = T1(1.0);
  this->stepsize_decrease = T1(2.0);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("hooke_jeeves_algorithm_state", "[hooke_jeeves_algorithm][hooke_jeeves_algorithm_state]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::hooke_jeeves_algorithm_state<value_type, number_of_dimensions> hooke_jeeves_algorithm_state;
  
  CHECK(hooke_jeeves_algorithm_state.stepsize == Approx(1.0));
}

TEST_CASE("hooke_jeeves_algorithm", "[hooke_jeeves_algorithm]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::hooke_jeeves_algorithm<value_type, number_of_dimensions> hooke_jeeves_algorithm;
  const mant::hooke_jeeves_algorithm_state<value_type, number_of_dimensions> hooke_jeeves_algorithm_state;
  
  CHECK(hooke_jeeves_algorithm.initial_stepsize == 1.0);
  CHECK(hooke_jeeves_algorithm.stepsize_decrease == 2.0);
}
#endif
