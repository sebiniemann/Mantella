/**

*/
template <
  typename T,
  std::size_t N>
struct optimisation_problem {
  typedef T value_type;
  typedef decltype(N) size_type;
  
  typedef std::function<T(
      const std::array<T, N>& parameter)> objective_function_type;

  std::vector<std::pair<objective_function_type, std::string>> objective_functions;
  std::array<T, N> lower_bounds;
  std::array<T, N> upper_bounds;
  
  optimisation_problem() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
optimisation_problem<T, number_of_dimensions>::optimisation_problem() noexcept {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  // These bounds could be anything as default, as they need user-given, context-dependent knowledge to be useful.
  // However, since we map the bounds later on internally to [0, 1] to ease maintainability in our optimisation 
  // algorithms, we use [0, 1] as default.
  lower_bounds.fill(T(0.0));
  upper_bounds.fill(T(1.0));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimisation_problem", "[optimisation_problem]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 2;
  const mant::optimisation_problem<value_type, number_of_dimensions> optimisation_problem;
  
  SECTION("Default values") {
    CHECK((optimisation_problem.lower_bounds == std::array<value_type, number_of_dimensions>({0.0, 0.0})));
    CHECK((optimisation_problem.upper_bounds == std::array<value_type, number_of_dimensions>({1.0, 1.0})));
  }
  
  SECTION("Objective functions") {
    CHECK(optimisation_problem.objective_functions.empty() == true);
  }
}
#endif
