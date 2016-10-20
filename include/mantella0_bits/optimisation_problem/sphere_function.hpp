/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct sphere_function : optimisation_problem<T, number_of_dimensions> {
  constexpr sphere_function() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr sphere_function<T, number_of_dimensions>::sphere_function() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  /*   n   /                \
   *  sum  | parameter(i)^2 |
   * i = 1 \                /
   */
  this->objective_functions = {{
    [](
        const auto& parameter) {
      return 
        std::accumulate(
          parameter.cbegin(), parameter.cend(),
          T(0.0),
          [](const double length, const double element) {
            return length + std::pow(element, T(2.0));
          });
    },
    "sphere function"}};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sphere_function", "[optimisation_problem][sphere_function]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::sphere_function<value_type, number_of_dimensions> sphere_function;
  
  CHECK(sphere_function.objective_functions.size() == 1);
  CHECK(std::get<0>(sphere_function.objective_functions.at(0))({1.0, -2.0, 3.0}) == Approx(14.0));
  CHECK(std::get<1>(sphere_function.objective_functions.at(0)) == "sphere function");
}
#endif
