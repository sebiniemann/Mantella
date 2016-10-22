/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct rosenbrock_function : optimisation_problem<T, number_of_dimensions> {
  using typename optimisation_problem<T, number_of_dimensions>::value_type;
  using typename optimisation_problem<T, number_of_dimensions>::size_type;
  
  rosenbrock_function() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
rosenbrock_function<T, number_of_dimensions>::rosenbrock_function() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");

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
      return std::inner_product(
        parameter.cbegin(), std::prev(parameter.cend(), 1),
        std::next(parameter.cbegin(), 1),
        T(0.0),
        std::plus<T>(),
        [](const auto element, const auto other_element) {
          return 
            T(100.0) * std::pow(
              other_element + 1 - std::pow(element + 1, T(2.0)),
              T(2.0)
            ) + 
            std::pow(element, T(2.0));
        });
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
  
  CHECK(rosenbrock_function.objective_functions.size() == 1);
  CHECK(std::get<0>(rosenbrock_function.objective_functions.at(0))({1.0, -2.0, 3.0}) == Approx(3405.0));
  CHECK(std::get<1>(rosenbrock_function.objective_functions.at(0)) == "Rosenbrock function");
}
#endif
