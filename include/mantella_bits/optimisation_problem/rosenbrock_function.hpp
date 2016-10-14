#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct rosenbrock_function : optimisation_problem<T, number_of_dimensions> {
  constexpr rosenbrock_function() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr rosenbrock_function<T, number_of_dimensions>::rosenbrock_function() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  static_assert(number_of_dimensions > 1, "");

  /* @see Howard H. Rosenbrock (1960). An automatic method for finding the greatest or least value of a function. The 
   * Computer Journal, 3(3), pp. 175–184.
   *
   *       /                                                                          \
   *   n   |       /                                             \^2                  |
   *  sum  | 100 * | parameter(i + 1) + 1 - (parameter(i) + 1)^2 |   + parameter(i)^2 |
   * i = 1 |       \                                             /                    |
   *       \                                                                          /
   */
  this->objective_functions = {{
    [](
        const auto& parameter) {
      T sum = T(0.0);
      for (std::size_t n = 0; n < number_of_dimensions - 1; ++n) {
        sum += 
          T(100.0) *
          std::pow(
            parameter.at(n + 1) + T(1.0) - 
            std::pow(parameter.at(n) + T(1.0), T(2.0)), 
            T(2.0)
          ) + 
          std::pow(parameter.at(n), T(2.0));
      }
      return sum;
    },
    "Rosenbrock function"}};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rosenbrock_function", "[optimisation_problem][rosenbrock_function]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::rosenbrock_function<value_type, number_of_dimensions> rosenbrock_function;
  
  // Checks that there is only one objective function as default.
  CHECK(rosenbrock_function.objective_functions.size() == 1);
  // Checks that the objective function returns the expected objective value.
  CHECK(std::get<0>(rosenbrock_function.objective_functions.at(0))({1.0, -2.0, 3.0}) == Approx(3405.0));
  // Checks that the objective function is named "Rosenbrock function".
  CHECK(std::get<1>(rosenbrock_function.objective_functions.at(0)) == "Rosenbrock function");
}
#endif
