#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct rastrigin_function : optimisation_problem<T, number_of_dimensions> {
  constexpr rastrigin_function() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr rastrigin_function<T, number_of_dimensions>::rastrigin_function() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  /* Original, 2-dimensional:  @see L. A. Rastrigin (1974). Systems of Extremal Control.
   * Generalised, n-dimensional: @see H. Mühlenbein, D. Schomisch and J. Born (1991). The Parallel Genetic Algorithm as 
   * Function Optimizer. Parallel Computing, 17(6-7), pp. 619–632.
   *
   *            n   /                                                  \
   * 10 * N +  sum  | parameter(i)^2 - 10 * cos(2 * pi * parameter(i)) |
   *          i = 1 \                                                  /
   */
  this->objective_functions = {{
    [](
        const auto& parameter) {
      return 
        T(10.0) *
        static_cast<T>(number_of_dimensions) +
        std::accumulate(
          parameter.cbegin(), parameter.cend(),
          T(0.0),
          [](
              const T sum,
              const T element) {
            return 
              sum +
              std::pow(element, T(2.0)) - 
              T(10.0) * std::cos(T(2.0) * std::acos(T(-1.0)) * element);
          });
    },
    "Rastrigin function"}};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rastrigin_function", "[optimisation_problem][rastrigin_function]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::rastrigin_function<value_type, number_of_dimensions> rastrigin_function;
  
  CHECK(rastrigin_function.objective_functions.size() == 1);
  CHECK(std::get<0>(rastrigin_function.objective_functions.at(0))({1.0, -2.0, 3.0}) == Approx(14.0));
  CHECK(std::get<1>(rastrigin_function.objective_functions.at(0)) == "Rastrigin function");
}
#endif
