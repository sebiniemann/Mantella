/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct random_search : optimisation_algorithm<T, number_of_dimensions> {
  using typename optimisation_algorithm<T, number_of_dimensions>::state_type;
  using typename optimisation_algorithm<T, number_of_dimensions>::value_type;
  
  constexpr random_search() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr random_search<T, number_of_dimensions>::random_search() noexcept 
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
    "Draws all parameters randomly and uniformly from [0, 1]."
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_search", "[random_search]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  mant::random_search<value_type, number_of_dimensions> random_search;
  mant::optimisation_algorithm_state<value_type, number_of_dimensions> random_search_state;
  
  SECTION("Next parameters functions") {
    CHECK(random_search.next_parameters_functions.size() == 1);
    CHECK(std::get<1>(random_search.next_parameters_functions.at(0)) == "Draws all parameters randomly and uniformly from [0, 1].");
    
    random_search.active_dimensions = {0, 2};
    random_search_state.parameters.resize(2);
    
    std::get<0>(random_search.next_parameters_functions.at(0))(random_search_state);
    
    CHECK(random_search_state.parameters.size() == 2);
    // Checks that all elements are within [0, 1].
    for (const auto& parameter : random_search_state.parameters) {
      CHECK(std::all_of(
        parameter.cbegin(), std::next(parameter.begin(), random_search.active_dimensions.size()),
        [](const auto element) {
          return (0.0 <= element && element <= 1.0);
        }) == true);
    }
  }
}
#endif
