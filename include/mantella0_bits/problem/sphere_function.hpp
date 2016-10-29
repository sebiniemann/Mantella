/**

*/
template <typename T, unsigned N>
struct sphere_function : problem<T, N> {
  sphere_function() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
sphere_function<T, N>::sphere_function() noexcept 
    : problem<T, N>() {
  /*   n   /                \
   *  sum  | parameter(i)^2 |
   * i = 1 \                /
   */
  this->objective_function = [](const auto& parameter) {
    return 
      std::accumulate(
        parameter.cbegin(), parameter.cend(),
        T(0.0),
        [](const double length, const double element) {
          return length + std::pow(element * T(10.24) - T(5.12), T(2.0));
        });
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sphere_function", "[problem][sphere_function]") {
  const mant::sphere_function<double, 3> sphere_function;
  
  CHECK(sphere_function.objective_function({1.0, -2.0, 3.0}) == Approx(1336.9344));
}
#endif
