#pragma once

template <typename T1, typename T2, typename T3>
bool is_almost_equal(
    const T1 first_value,
    const T2 second_value,
    const T3 units_in_last_place);

template <typename T1, typename T2>
bool is_almost_equal(
    const T1 first_value,
    const T2 second_value);

//
// Implementation
//

template <typename T1, typename T2, typename T3>
bool is_almost_equal(
    const T1 first_value,
    const T2 second_value,
    const T3 epsilon) {
  static_assert(std::is_floating_point<T1>::value, "The first value must be a floating point.");
  static_assert(std::is_floating_point<T2>::value, "The second value must be a floating point.");
  static_assert(std::is_floating_point<T3>::value, "Epsilon must be a floating point.");
  
    // Handle NaN.
    if (std::isnan(a) || std::isnan(b))
        return false;

    typedef std::numeric_limits<T> limits;

    // Handle very small and exactly equal values.
    if (std::abs(a-b) <= ulps * limits::denorm_min())
        return true;

    // frexp() does the wrong thing for zero.  But if we get this far
    // and either number is zero, then the other is too big, so just
    // handle that now.
    if (a == 0 || b == 0)
        return false;

    // Break the numbers into significand and exponent, sorting them by
    // exponent.
    int min_exp, max_exp;
    T min_frac = my_frexp(a, &min_exp);
    T max_frac = my_frexp(b, &max_exp);
    if (min_exp > max_exp)
    {
        std::swap(min_frac, max_frac);
        std::swap(min_exp, max_exp);
    }

    // Convert the smaller to the scale of the larger by adjusting its
    // significand.
    const T scaled_min_frac = std::ldexp(min_frac, min_exp-max_exp);

    // Since the significands are now in the same scale, and the larger
    // is in the range [0.5, 1), 1 ulp is just epsilon/2.
    return std::abs(max_frac-scaled_min_frac) <= ulps * limits::epsilon() / 2;
}

template <typename T1, typename T2>
bool is_almost_equal(
    const T1 first_value,
    const T2 second_value) {
  return is_almost_equal(first_value, second_value, std::numeric_limits<typename std::common_type<T1, T2>::type>::epsilon());
}

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("is_almost_equal", "[is_almost_equal]") {
  CHECK(is_almost_equal(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()) == false);

  CHECK(is_almost_equal(0.3f, 0.3, 1e-8) == false);
  CHECK(is_almost_equal(0.3f, 0.3, 1e-7) == true);
  CHECK(is_almost_equal(0.951, 1.0, 0.05) == true);
}
#endif