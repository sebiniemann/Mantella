#pragma once

template <
  typename T,
  std::size_t number_of_dimensions>
struct analyse_result {
  std::array<std::size_t, number_of_dimensions> additive_separability;
};

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
analyse_result<T1, number_of_dimensions> analyse(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const std::size_t number_of_evaluations,
    const T1 acceptable_deviation);

//
// Implementation
//

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
analyse_result<T1, number_of_dimensions> analyse(
    const T2<T1, number_of_dimensions>& optimisation_problem,
    const std::size_t number_of_evaluations,
    const T1 acceptable_deviation) {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<mant::optimisation_problem<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  assert(number_of_evaluations > 0);
  assert(acceptable_deviation >= 0);

  return {
    additive_separability(optimisation_problem, number_of_evaluations, acceptable_deviation)
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("analyse", "[analyse]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 5;
  const mant::sphere_function<value_type, number_of_dimensions> sphere_function;

  const auto&& result = mant::analyse(sphere_function, 100, 1e-12);
  CHECK((result.additive_separability == std::array<std::size_t, 5>({0, 1, 2, 3, 4})));
}
#endif