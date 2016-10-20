/**

*/
template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
T1 evaluate(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const std::array<T1, number_of_dimensions>& parameter);
    
//
// Implementation
//  

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
T1 evaluate(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const std::array<T1, number_of_dimensions>& parameter) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<mant::optimisation_problem<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  assert(optimisation_problem.objective_functions.size() > 0);
      
  return std::accumulate(
    optimisation_problem.objective_functions.cbegin(), optimisation_problem.objective_functions.cend(),
    T1(0.0),
    [&parameter](const T1 sum, const auto objective_function) {
      return sum + objective_function.first(parameter);
    });
}
    
//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("evaluate", "[evaluate]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  mant::sphere_function<value_type, number_of_dimensions> sphere_function;
  
  // Adds another objective function, testing that all objectives are summed up.
  sphere_function.objective_functions.push_back({
    [](const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), 1.0, std::multiplies<double>());
    },
    "My other objective function"
  });
  
  CHECK(mant::evaluate(sphere_function, {2.0, -3.0, 4.0}) == Approx(5.0));
}
#endif
