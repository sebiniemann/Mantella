#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct ackley_function : optimisation_problem<T, number_of_dimensions> {
  constexpr ackley_function() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr ackley_function<T, number_of_dimensions>::ackley_function() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  /* @see David H. Ackley (1987). A Connectionist Machine for Genetic Hillclimbing. Kluwer Academic Publishers.
   *
   *      /                                       \       /   N                              \
   *      |      / -0.2 * norm_2(parameter)     \ |       |  sum  cos(2 * pi * parameter(i)) | 
   * 20 * | -exp | ------------------------ + 1 | | - exp | i = 1                            | + exp(1)
   *      |      \      sqrt(N)                 / |       | -------------------------------- |
   *      \                                       /       \                 N                /
   */
  this->objective_functions = {{
    [](
        const auto& parameter) {
      return
        T(20.0) * (
          -std::exp(
            T(-0.2) * cblas_dnrm2(number_of_dimensions, parameter.data(), 1) /
            std::sqrt(number_of_dimensions)
          ) + 
          T(1.0)
        ) -
        std::exp(
          std::accumulate(
            parameter.cbegin(),
            parameter.cend(),
            T(0.0),
            [](
                const auto sum,
                const auto element) {
              return 
                sum +
                std::cos(T(2.0) * std::acos(T(-1.0)) * element);
            }
          ) / 
          static_cast<T>(number_of_dimensions)
        ) + 
        std::exp(T(1.0));
    },
    "Ackley's function"}};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("ackley_function", "[optimisation_problem][ackley_function]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  const mant::ackley_function<value_type, number_of_dimensions> ackley_function;
  
  // Checks that there is only one objective function as default.
  CHECK(ackley_function.objective_functions.size() == 1);
  // Checks that the objective function returns the expected objective value.
  CHECK(std::get<0>(ackley_function.objective_functions.at(0))({1.0, -2.0, 3.0}) == Approx(7.016453608269399));
  // Checks that the objective function is named "Ackley's function".
  CHECK(std::get<1>(ackley_function.objective_functions.at(0)) == "Ackley's function");
}
#endif
