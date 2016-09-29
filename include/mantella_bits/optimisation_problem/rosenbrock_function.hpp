#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct rosenbrock_function_t : optimisation_problem_t<T, number_of_dimensions> {
  constexpr rosenbrock_function_t() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr rosenbrock_function_t<T, number_of_dimensions>::rosenbrock_function_t() noexcept 
    : optimisation_problem_t<T, number_of_dimensions>() {
  this->objective_functions = {{
    [](
        const auto& parameter) {
      T sum = T(0.0);
      for (std::size_t n = 0; n < number_of_dimensions - 1; ++n) {
        sum += T(100.0) * std::pow(parameter[n + 1] + T(1.0) - std::pow(parameter[n] + T(1.0), T(2.0)), T(2.0)) + std::pow(parameter[n], T(2.0));
      }
      return sum;
    },
    "rosenbrock function"
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rosenbrock_function", "[optimisation_problem][rosenbrock_function]") {
  CHECK((mant::rosenbrock_function_t<double, 3>().objective_functions[0].first({1.0, -2.0, 3.0}) == Approx(3405.0)));
}
#endif
