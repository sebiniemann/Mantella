#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct sphere_function_t : optimisation_problem<T, number_of_dimensions> {
  constexpr sphere_function_t() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr sphere_function_t<T, number_of_dimensions>::sphere_function_t() noexcept 
    : optimisation_problem<T, number_of_dimensions>() {
  this->objective_functions = {{
    [](
        const auto& parameter) {
      return std::accumulate(parameter.cbegin(), parameter.cend(), T(0.0), [](const double length, const double element) {return length + std::pow(element, T(2.0));});
    },
    "sphere function"
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sphere_function", "[optimisation_problem][sphere_function]") {
  CHECK((mant::sphere_function_t<double, 3>().objective_functions[0].first({1.0, -2.0, 3.0}) == Approx(14.0)));
}
#endif