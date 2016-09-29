#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct rastrigin_function_t : optimisation_problem_t<T, number_of_dimensions> {
  constexpr rastrigin_function_t() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr rastrigin_function_t<T, number_of_dimensions>::rastrigin_function_t() noexcept 
    : optimisation_problem_t<T, number_of_dimensions>() {
  this->objective_functions = {{
    [](
        const auto& parameter) {
      return T(10.0) * static_cast<T>(number_of_dimensions) + std::accumulate(parameter.cbegin(), parameter.cend(), T(0.0), [](const T sum, const T element) {return sum + std::pow(element, T(2.0)) - T(10.0) * std::cos(T(2.0) * std::acos(T(-1.0)) * element);});
    },
    "rastrigin function"
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("rastrigin_function", "[optimisation_problem][rastrigin_function]") {
  CHECK((mant::rastrigin_function_t<double, 3>().objective_functions[0].first({1.0, -2.0, 3.0}) == Approx(14.0)));
}
#endif
