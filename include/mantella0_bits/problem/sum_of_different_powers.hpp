/**
Sum of different powers
=======================

.. cpp:class:: sum_of_different_powers : public problem

  .. versionadded:: 1.0.0 

  .. list-table:: Template parameters
    :widths: 27 73

    * - T
        
        Any floating point type
      - The value type of the parameter and objective value.
    * - N
        
        ``unsigned``
      - The number of dimensions.
        
        Must be within ``[1, std::numeric_limits<unsigned>::max()]``.
      
  .. list-table:: Member functions
    :widths: 27 73
    
    * - sum_of_different_powers
    
        (Constructor)
      - Initialises all member variables to their default value.
      
        Will never throw an exception.
*/
template <typename T,  unsigned N>
struct sum_of_different_powers : problem<T, N> {
  sum_of_different_powers() noexcept;
};

//
// Implementation
//

template <typename T, unsigned N>
sum_of_different_powers<T, N>::sum_of_different_powers() noexcept 
    : problem<T, N>() {
  /*   n   /                           \
   *  sum  | abs(parameter(i))^(i + 1) |
   * i = 1 \                           /
   */
  this->objective_function = [](const auto& parameter) {
    T sum = T(0.0);
    for (unsigned n = 0; n < N; ++n) {
      sum += std::pow(std::fabs(parameter.at(n) * T(2.0) - T(1.0)), static_cast<T>(n + 2));
    }
    
    return sum;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("sum_of_different_powers", "[problem][sum_of_different_powers]") {
  const mant::sum_of_different_powers<double, 3> sum_of_different_powers;
  
  CHECK(sum_of_different_powers.objective_function({1.0, -2.0, 3.0}) == Approx(751.0));
}
#endif
