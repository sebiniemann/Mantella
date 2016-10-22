/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct random_search : optimisation_algorithm<T, number_of_dimensions> {
  random_search() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
random_search<T, number_of_dimensions>::random_search() noexcept 
    : optimisation_algorithm<T, number_of_dimensions>() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  this->next_parameters_functions = {{
    [this](auto& state) {
      for (auto& parameter : state.parameters) {
        std::generate(
          parameter.begin(), std::next(parameter.begin(), this->active_dimensions.size()),
          std::bind(
            std::uniform_real_distribution<T>(0.0, 1.0),
            std::ref(random_number_generator())));
      }
    },
    "Random search next parameters"
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_search", "[random_search]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::random_search<value_type, number_of_dimensions> random_search;
  const mant::optimisation_algorithm_state<value_type, number_of_dimensions> random_search_state;
  
  // Nothing to test here.
}
#endif
