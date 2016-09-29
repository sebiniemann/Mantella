#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct sum_of_different_powers_function_t : optimisation_problem_t<T, number_of_dimensions> {
  constexpr sum_of_different_powers_function_t() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr sum_of_different_powers_function_t<T, number_of_dimensions>::sum_of_different_powers_function_t() noexcept 
    : optimisation_problem_t<T, number_of_dimensions>() {
  this->lower_bounds.fill(-30.0);
  this->upper_bounds.fill(30.0);
  this->objective_functions = {{
    [](
        const auto& parameter) {
      T sum = T(0.0);
      for (std::size_t n = 0; n < number_of_dimensions; ++n) {
        sum += std::pow(std::abs(parameter[n]), n + 2);
      }
      return sum;
    },
    "sum of different powers function"
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sum_of_different_powers_function", "[optimisation_problem][sum_of_different_powers_function]") {
  CHECK((mant::sum_of_different_powers_function_t<double, 3>().objective_functions[0].first({1.0, -2.0, 3.0}) == Approx(90.0)));
}
#endif