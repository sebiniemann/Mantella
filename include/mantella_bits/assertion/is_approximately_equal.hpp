#pragma once

template <typename T1, typename T2, typename T3>
bool is_approximately_equal(
    const T1 first_value,
    const T2 second_value,
    const T3 epsilon);

template <typename T1, typename T2>
bool is_approximately_equal(
    const T1 first_value,
    const T2 second_value);

//
// Implementation
//

template <typename T1, typename T2, typename T3>
bool is_approximately_equal(
    const T1 first_value,
    const T2 second_value,
    const T3 epsilon) {
  static_assert(std::is_floating_point<T1>::value, "The first value must be a floating point.");
  static_assert(std::is_floating_point<T2>::value, "The second value must be a floating point.");
  static_assert(std::is_floating_point<T3>::value, "Epsilon must be a floating point.");
  assert(epsilon >= 0.0);
  
  if (std::isnan(first_value) || std::isnan(second_value)) {
    return false;
  }
  
  if (std::isinf(first_value) && std::isinf(second_value)) {
    return std::signbit(first_value) == std::signbit(second_value);
  } else if (std::isinf(first_value) != std::isinf(second_value)) {
    return std::fabs(first_value - second_value) <= epsilon;
  }
  
  return std::fabs(first_value - second_value) <= std::fmax(std::fabs(first_value), std::fabs(second_value)) * epsilon;
}

template <typename T1, typename T2>
bool is_approximately_equal(
    const T1 first_value,
    const T2 second_value) {
  return is_approximately_equal(first_value, second_value, std::numeric_limits<typename std::common_type<T1, T2>::type>::epsilon());
}

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("is_approximately_equal", "[is_approximately_equal]") {
  SECTION("T1, T2, T3") {
    // Each parameter can have a different floating point type.
    CHECK(is_approximately_equal(1.0f, 2.0, 1e-12l) == false);
    
    // NaN is not equal to NaN.
    CHECK(is_approximately_equal(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), 1e-12) == false);
    
    // Infinities are equal if they have the same sign.
    CHECK(is_approximately_equal(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), 1e-12) == true);
    CHECK(is_approximately_equal(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), 1e-12) == false);
    
    // Returns false if epsilon is smaller than `10^-std::numeric_limits<T>::digits10` 
    CHECK(is_approximately_equal(0.3f, 0.3, 1e-7) == true);
    CHECK(is_approximately_equal(0.3f, 0.3, 1e-8) == false);
    CHECK(is_approximately_equal(0.3, 0.3, 1e-8) == true);
    
    CHECK(is_approximately_equal(0.951, 1.0, 0.05) == true);
    CHECK(is_approximately_equal(1.0, std::numeric_limits<double>::infinity()) == false);
    
    CHECK(is_approximately_equal(1.0, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()) == false);
  }
  
  SECTION("T1, T2") {
    // Each parameter can have a different floating point type.
    CHECK(is_approximately_equal(1.0f, 2.0) == false);
  }
}
#endif