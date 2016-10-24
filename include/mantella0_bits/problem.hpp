/**

*/
template <typename T, std::size_t N>
struct problem {
  std::function<T(
      const std::array<T, N>& parameter)> objective_function;
  
  std::array<T, N> lower_bounds;
  std::array<T, N> upper_bounds;
  
  constexpr problem() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
constexpr problem<T, N>::problem() noexcept {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(N > 0, "");
  
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
TEST_CASE("problem", "[problem]") {
  const mant::problem<double, 2> problem;
  
  CHECK((problem.lower_bounds == std::array<double, 2>({0.0, 0.0})));
  CHECK((problem.upper_bounds == std::array<double, 2>({1.0, 1.0})));
}
#endif
