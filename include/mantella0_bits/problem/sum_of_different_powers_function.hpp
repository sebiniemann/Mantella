/**

*/
template <typename T,  std::size_t N>
struct sum_of_different_powers_function : problem<T, N> {
  sum_of_different_powers_function() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
sum_of_different_powers_function<T, N>::sum_of_different_powers_function() noexcept 
    : problem<T, N>() {
  /*   n   /                           \
   *  sum  | abs(parameter(i))^(i + 1) |
   * i = 1 \                           /
   */
  this->objective_function = [](const auto& parameter) {
    T sum = T(0.0);
    for (std::size_t n = 0; n < N; ++n) {
      sum += std::pow(std::fabs(parameter.at(n) * T(2.0) - T(1.0)), static_cast<T>(n + 2));
    }
    
    return sum;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sum_of_different_powers_function", "[problem][sum_of_different_powers_function]") {
  const mant::sum_of_different_powers_function<double, 3> sum_of_different_powers_function;
  
  CHECK(sum_of_different_powers_function.objective_function({1.0, -2.0, 3.0}) == Approx(751.0));
}
#endif
