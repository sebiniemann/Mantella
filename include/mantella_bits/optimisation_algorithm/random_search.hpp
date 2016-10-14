#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct random_search : optimisation_algorithm<T, number_of_dimensions> {
  constexpr random_search() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr random_search<T, number_of_dimensions>::random_search() noexcept 
    : optimisation_algorithm<T, number_of_dimensions>() {
  this->next_parameters_functions = {{
    [this](
        auto state) {
      std::generate(
        state.parameters.begin(),
        state.parameters.end(),
        [this]() {
          std::array<T, number_of_dimensions> parameter;
          std::generate(
            parameter.begin(),
            std::next(parameter.begin(), this->active_dimensions.size()),
            std::bind(std::uniform_real_distribution<T>(0.0, 1.0), std::ref(random_number_generator()))
          );
          
          return parameter;
        }
      );
          
      return state;
    },
    "Draws all parameters randomly and uniformly from [*lower_bounds*, *upper_bounds*]."
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("random_search", "[random_search]") {
  
}
#endif
