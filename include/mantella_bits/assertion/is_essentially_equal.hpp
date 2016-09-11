#pragma once

template <typename T1, typename T2, typename T3>
bool is_essentially_equal(
    const T1 first_value,
    const T2 second_value,
    const T3 epsilon);

template <typename T1, typename T2>
bool is_essentially_equal(
    const T1 first_value,
    const T2 second_value);

//
// Implementation
//

template <typename T1, typename T2, typename T3>
bool is_essentially_equal(
    const T1 first_value,
    const T2 second_value,
    const T3 epsilon) {
  static_assert(std::is_floating_point<T1>::value, "The first value must be a floating point.");
  static_assert(std::is_floating_point<T2>::value, "The second value must be a floating point.");
  static_assert(std::is_floating_point<T3>::value, "Epsilon must be a floating point.");
  
  return std::fabs(first_value - second_value) <= std::fmin(std::fabs(first_value), std::fabs(second_value)) * epsilon;
}

template <typename T1, typename T2>
bool is_essentially_equal(
    const T1 first_value,
    const T2 second_value) {
  return is_essentially_equal(first_value, second_value, std::numeric_limits<typename std::common_type<T1, T2>::type>::epsilon());
}

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("is_essentially_equal", "[is_essentially_equal]") {
  CHECK(is_essentially_equal(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()) == false);

  CHECK(is_essentially_equal(0.3f, 0.3, 1e-8) == false);
  CHECK(is_essentially_equal(0.3f, 0.3, 1e-7) == true);
  CHECK(is_essentially_equal(0.951, 1.0, 0.05) == false);
}
#endif