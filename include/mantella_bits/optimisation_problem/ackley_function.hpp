#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct ackley_function_t : optimisation_problem_t<T, number_of_dimensions> {
  constexpr ackley_function_t() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr ackley_function_t<T, number_of_dimensions>::ackley_function_t() noexcept 
    : optimisation_problem_t<T, number_of_dimensions>() {
  this->objective_functions = {{
    [](
        const auto& parameter) {
      return T(20.0) * (-std::exp(T(-0.2) * cblas_dnrm2(number_of_dimensions, parameter.data(), 1) / std::sqrt(number_of_dimensions)) + T(1.0)) - std::exp(std::accumulate(parameter.cbegin(), parameter.cend(), T(0.0), [](const T sum, const T element) {return sum + std::cos(T(2.0) * std::acos(T(-1.0)) * element);}) / static_cast<T>(number_of_dimensions)) + std::exp(T(1.0));
    },
    "ackley function"
  }};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("ackley_function", "[optimisation_problem][ackley_function]") {
  CHECK((mant::ackley_function_t<double, 3>().objective_functions[0].first({1.0, -2.0, 3.0}) == Approx(7.016453608269399)));
}
#endif
