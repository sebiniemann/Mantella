#pragma once

template <typename T, std::size_t number_of_dimensions>
struct analyse_result {
  std::array<std::size_t, number_of_dimensions> additive_separability;
};

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2>
analyse_result<T1, number_of_dimensions> analyse(
    const T2<T1, number_of_dimensions>& optimisation_problem);
    
//
// Implementation
//

template <typename T1, std::size_t number_of_dimensions, template <class, std::size_t> class T2>
analyse_result<T1, number_of_dimensions> analyse(
    const T2<T1, number_of_dimensions>& optimisation_problem) {
  return {additive_separability(optimisation_problem)};
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("analyse", "[analyse]") {
  const auto&& result = mant::analyse(mant::sphere_function<double, 5>());
  CHECK((result.additive_separability == std::array<std::size_t, 5>({0, 1, 2, 3, 4})));
}
#endif