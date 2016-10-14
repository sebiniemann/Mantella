#pragma once

/**

*/
template <
  typename T, 
  std::size_t number_of_dimensions>
struct sum_of_different_powers_function : optimisation_problem<T, number_of_dimensions> {
  constexpr sum_of_different_powers_function() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr sum_of_different_powers_function<T, number_of_dimensions>::sum_of_different_powers_function() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  /*   n   /                           \
   *  sum  | abs(parameter(i))^(i + 1) |
   * i = 1 \                           /
   */
  this->objective_functions = {{
    [](
        const auto& parameter) {
      T sum = T(0.0);
      for (std::size_t n = 0; n < number_of_dimensions; ++n) {
        sum += std::pow(std::fabs(parameter.at(n)), static_cast<T>(n + 2));
      }
      return sum;
    },
    "sum of different powers function"}};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sum_of_different_powers_function", "[optimisation_problem][sum_of_different_powers_function]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::sum_of_different_powers_function<value_type, number_of_dimensions> sum_of_different_powers_function;
  
  // Checks that there is only one objective function as default.
  CHECK(sum_of_different_powers_function.objective_functions.size() == 1);
  // Checks that the objective function returns the expected objective value.
  CHECK(std::get<0>(sum_of_different_powers_function.objective_functions.at(0))({1.0, -2.0, 3.0}) == Approx(90.0));
  // Checks that the objective function is named "sum of different powers function".
  CHECK(std::get<1>(sum_of_different_powers_function.objective_functions.at(0)) == "sum of different powers function");
}
#endif