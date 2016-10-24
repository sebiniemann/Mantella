/**

*/
template <typename T, std::size_t N>
struct ackley_function : problem<T, N> {
  ackley_function() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
ackley_function<T, N>::ackley_function() noexcept 
    : problem<T, N>() {
  /* @see David H. Ackley (1987). A Connectionist Machine for Genetic Hillclimbing. Kluwer Academic Publishers.
   *
   *      /                                     \       /   N                              \
   *      |      / -0.2 * norm(parameter)     \ |       |  sum  cos(2 * pi * parameter(i)) | 
   * 20 * | -exp | ---------------------- + 1 | | - exp | i = 1                            | + exp(1)
   *      |      \      sqrt(N)               / |       | -------------------------------- |
   *      \                                     /       \                 N                /
   */
  this->objective_function = [](const auto& parameter) {
    return
      T(20.0) * (
        -std::exp(
          T(-0.2) * std::sqrt(std::inner_product(
            parameter.cbegin(), parameter.cend(),
            parameter.cbegin(),
            T(0.0))
          ) /
          std::sqrt(N)
        ) + 
        T(1.0)
      ) -
      std::exp(
        std::accumulate(
          parameter.cbegin(), parameter.cend(),
          T(0.0),
          [](const auto sum, const auto element) {
            return 
              sum +
              std::cos(T(2.0) * std::acos(T(-1.0)) * element);
          }
        ) / 
        static_cast<T>(N)
      ) + 
      std::exp(T(1.0));
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("ackley_function", "[problem][ackley_function]") {
  const mant::ackley_function<double, 3> ackley_function;
  
  CHECK(ackley_function.objective_function({1.0, -2.0, 3.0}) == Approx(7.016453608269399));
}
#endif
